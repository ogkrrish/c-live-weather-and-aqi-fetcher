#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESPONSE_SIZE 10000

// Function to URL encode a string
void url_encode(const char *src, char *dst, size_t dst_size) {
    const char *hex = "0123456789ABCDEF";
    size_t pos = 0;

    while (*src && pos < dst_size - 4) {
        if (isalnum(*src) || *src == '-' || *src == '_' || *src == '.' ||
            *src == '~') {
            dst[pos++] = *src;
        }
        else if (*src == ' ') {
            dst[pos++] = '+';
        }
        else {
            dst[pos++] = '%';
            dst[pos++] = hex[(*src >> 4) & 0xF];
            dst[pos++] = hex[*src & 0xF];
        }
        src++;
    }
    dst[pos] = '\0';
}

// Function to fetch URL using system curl command
int http_get_via_curl(const char *url, char *response, size_t max_size) {
    char cmd[1024];
    char temp_file[] = "weather_temp.json";
    FILE *f;
    size_t bytes_read;

    // Build curl command to save output to file
    sprintf(cmd, "curl -s -o %s \"%s\"", temp_file, url);

    // Execute curl
    int result = system(cmd);
    if (result != 0) {
        return 0;
    }

    // Read the response file
    f = fopen(temp_file, "r");
    if (!f) {
        return 0;
    }

    bytes_read = fread(response, 1, max_size - 1, f);
    response[bytes_read] = '\0';
    fclose(f);

    // Clean up temp file
    remove(temp_file);

    return bytes_read > 0 ? 1 : 0;
}

// Helper to extract nested JSON object
char *get_json_object(const char *json, const char *key) {
    static char object_buffer[5000];
    char search_key[256];
    sprintf(search_key, "\"%s\":{", key);
    char *start = strstr(json, search_key);
    if (!start)
        return NULL;

    // Move to the opening brace
    start = strchr(start, '{');
    if (!start)
        return NULL;

    // Find the matching closing brace
    int brace_count = 1;
    char *end = start + 1;
    while (*end && brace_count > 0) {
        if (*end == '{')
            brace_count++;
        else if (*end == '}')
            brace_count--;
        end++;
    }

    // Copy the object content (including braces)
    int len = end - start;
    if (len >= sizeof(object_buffer))
        len = sizeof(object_buffer) - 1;
    strncpy(object_buffer, start, len);
    object_buffer[len] = '\0';

    return object_buffer;
}

// Helper to get JSON array element
char *get_json_array_element(const char *json, const char *key, int index) {
    static char array_buffer[5000];
    char search_key[256];
    sprintf(search_key, "\"%s\":[", key);
    char *start = strstr(json, search_key);
    if (!start)
        return NULL;

    // Move to the opening bracket
    start = strchr(start, '[');
    if (!start)
        return NULL;
    start++; // Move past '['

    // Skip to the desired index
    int current_index = 0;
    int brace_count = 0;
    while (current_index < index && *start) {
        if (*start == '{')
            brace_count++;
        else if (*start == '}')
            brace_count--;
        else if (*start == ',' && brace_count == 0)
            current_index++;
        start++;
    }

    // Skip whitespace
    while (*start == ' ' || *start == '\t' || *start == '\n')
        start++;

    if (*start != '{')
        return NULL;

    // Find the matching closing brace
    brace_count = 1;
    char *end = start + 1;
    while (*end && brace_count > 0) {
        if (*end == '{')
            brace_count++;
        else if (*end == '}')
            brace_count--;
        end++;
    }

    // Copy the object content
    int len = end - start;
    if (len >= sizeof(array_buffer))
        len = sizeof(array_buffer) - 1;
    strncpy(array_buffer, start, len);
    array_buffer[len] = '\0';

    return array_buffer;
}

// Helper to get string value from JSON (extracts content between quotes)
int get_json_string(const char *json, const char *key, char *output,
                    size_t output_size) {
    char search_key[256];
    sprintf(search_key, "\"%s\":\"", key);
    char *start = strstr(json, search_key);
    if (!start)
        return 0;

    start += strlen(search_key);
    char *end = strchr(start, '"');
    if (!end)
        return 0;

    int len = end - start;
    if (len >= output_size)
        len = output_size - 1;
    strncpy(output, start, len);
    output[len] = '\0';

    return 1;
}

// JSON parser helpers
double get_json_double(const char *json, const char *key) {
    char search_key[256];
    sprintf(search_key, "\"%s\":", key);
    char *pos = strstr(json, search_key);
    if (pos) {
        pos += strlen(search_key);
        while (*pos == ' ' || *pos == '\t')
            pos++;
        return atof(pos);
    }
    return -999.0;
}

int get_json_int(const char *json, const char *key) {
    char search_key[256];
    sprintf(search_key, "\"%s\":", key);
    char *pos = strstr(json, search_key);
    if (pos) {
        pos += strlen(search_key);
        while (*pos == ' ' || *pos == '\t')
            pos++;
        return atoi(pos);
    }
    return -999;
}

// Weather description helper
const char *get_weather_desc(int code) {
    switch (code) {
    case 0:
        return "Clear sky";
    case 1:
        return "Mainly clear";
    case 2:
        return "Partly cloudy";
    case 3:
        return "Overcast";
    case 45:
    case 48:
        return "Foggy";
    case 51:
    case 53:
    case 55:
        return "Drizzle";
    case 61:
    case 63:
    case 65:
        return "Rain";
    case 71:
    case 73:
    case 75:
        return "Snow";
    case 77:
        return "Snow grains";
    case 80:
    case 81:
    case 82:
        return "Rain showers";
    case 85:
    case 86:
        return "Snow showers";
    case 95:
        return "Thunderstorm";
    case 96:
    case 99:
        return "Thunderstorm with hail";
    default:
        return "Unknown";
    }
}

// AQI category helper
const char *get_aqi_category(int aqi) {
    if (aqi <= 50)
        return "Good";
    else if (aqi <= 100)
        return "Moderate";
    else if (aqi <= 150)
        return "Unhealthy for Sensitive Groups";
    else if (aqi <= 200)
        return "Unhealthy";
    else if (aqi <= 300)
        return "Very Unhealthy";
    else
        return "Hazardous";
}

// Function to geocode a city name to coordinates
int geocode_city(const char *city_name, char *full_name, double *lat,
                 double *lon) {
    char response[MAX_RESPONSE_SIZE];
    char url[512];
    char encoded_city[256];

    url_encode(city_name, encoded_city, sizeof(encoded_city));
    sprintf(url,
            "http://geocoding-api.open-meteo.com/v1/"
            "search?name=%s&count=1&language=en&format=json",
            encoded_city);

    if (!http_get_via_curl(url, response, MAX_RESPONSE_SIZE)) {
        return 0;
    }

    // Extract first result from "results" array
    char *result = get_json_array_element(response, "results", 0);
    if (!result) {
        return 0;
    }

    // Extract latitude and longitude
    *lat = get_json_double(result, "latitude");
    *lon = get_json_double(result, "longitude");

    if (*lat == -999.0 || *lon == -999.0) {
        return 0;
    }

    // Get full name
    char name[128] = "";
    char country[64] = "";
    get_json_string(result, "name", name, sizeof(name));
    get_json_string(result, "country", country, sizeof(country));

    sprintf(full_name, "%s, %s", name, country);

    return 1;
}

void print_city_weather(const char *city, double lat, double lon) {
    char response[MAX_RESPONSE_SIZE];
    char url[512];

    printf("\n");
    printf("========================================\n");
    printf("  %s\n", city);
    printf("========================================\n");

    // Fetch Weather
    sprintf(url,
            "http://api.open-meteo.com/v1/"
            "forecast?latitude=%.4f&longitude=%.4f&current_weather=true",
            lat, lon);

    printf("  Fetching weather data... ");
    fflush(stdout);

    if (http_get_via_curl(url, response, MAX_RESPONSE_SIZE)) {
        printf("Done!\n");

        // Extract the nested "current_weather" object
        char *weather_obj = get_json_object(response, "current_weather");
        if (weather_obj) {
            double temp = get_json_double(weather_obj, "temperature");
            int code = get_json_int(weather_obj, "weathercode");
            double wind = get_json_double(weather_obj, "windspeed");

            if (temp != -999.0) {
                printf("  Temperature:    %.1f C\n", temp);
                printf("  Condition:      %s\n", get_weather_desc(code));
                printf("  Wind Speed:     %.1f km/h\n", wind);
            }
            else {
                printf("  [!] Failed to parse weather data\n");
            }
        }
        else {
            printf("  [!] Failed to extract weather object\n");
        }
    }
    else {
        printf("Failed!\n");
        printf("  [!] Could not fetch weather data\n");
    }

    // Fetch AQI
    sprintf(url,
            "http://air-quality-api.open-meteo.com/v1/"
            "air-quality?latitude=%.4f&longitude=%.4f&current=us_aqi",
            lat, lon);

    printf("  Fetching air quality... ");
    fflush(stdout);

    if (http_get_via_curl(url, response, MAX_RESPONSE_SIZE)) {
        printf("Done!\n");

        // Extract the nested "current" object for AQI
        char *aqi_obj = get_json_object(response, "current");
        if (aqi_obj) {
            int aqi = get_json_int(aqi_obj, "us_aqi");

            if (aqi != -999) {
                printf("  Air Quality:    %d - %s\n", aqi,
                       get_aqi_category(aqi));
            }
            else {
                printf("  [!] AQI data not available\n");
            }
        }
        else {
            printf("  [!] Failed to extract AQI object\n");
        }
    }
    else {
        printf("Failed!\n");
        printf("  [!] Could not fetch AQI data\n");
    }

    printf("========================================\n\n");
}

int main() {
    char city_input[256];
    char full_city_name[256];
    double lat, lon;

    printf("\n");
    printf("==========================================\n");
    printf("    Real-Time Weather Information App\n");
    printf("==========================================\n");
    printf("\n");

    while (1) {
        printf("Enter city name (or 'quit' to exit): ");
        fflush(stdout);

        // Read city name
        if (fgets(city_input, sizeof(city_input), stdin) == NULL) {
            break;
        }

        // Remove newline
        city_input[strcspn(city_input, "\n")] = 0;

        // Check for quit
        if (strcmp(city_input, "quit") == 0 ||
            strcmp(city_input, "exit") == 0 || strcmp(city_input, "q") == 0) {
            printf("\nThank you for using Weather App!\n");
            break;
        }

        // Skip empty input
        if (strlen(city_input) == 0) {
            continue;
        }

        printf("\nSearching for '%s'...\n", city_input);

        // Geocode the city
        if (geocode_city(city_input, full_city_name, &lat, &lon)) {
            printf("Found: %s (Lat: %.4f, Lon: %.4f)\n", full_city_name, lat,
                   lon);
            print_city_weather(full_city_name, lat, lon);
        }
        else {
            printf("\n[!] City not found. Please try again.\n");
            printf("    Examples: London, New York, Tokyo, Mumbai\n\n");
        }
    }

    return 0;
}
