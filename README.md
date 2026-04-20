<h1 align="center">🌦️ C Live Weather & AQI Fetcher</h1>

<p align="center">
  <b>Real-time Weather + Air Quality CLI app built in C</b><br>
  Minimal. Fast. No external libraries.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue?style=flat-square" alt="C">
  <img src="https://img.shields.io/badge/API-OpenMeteo-green?style=flat-square" alt="OpenMeteo API">
  <img src="https://img.shields.io/badge/Interface-CLI-black?style=flat-square" alt="CLI">
  <img src="https://img.shields.io/badge/Status-Stable-success?style=flat-square" alt="Stable">
</p>

<hr>

<h2>About</h2>
<p>
This project is a command-line application that retrieves real-time weather and air quality data using the Open-Meteo APIs.
</p>

<p>
It is designed as a low-level implementation using:
</p>

<ul>
  <li>System-level HTTP requests via <code>curl</code></li>
  <li>Manual JSON parsing without external libraries</li>
  <li>Clean terminal output for weather and AQI information</li>
</ul>

<hr>

<h2>Features</h2>

<ul>
  <li>🌍 Search weather by city name</li>
  <li>🌡️ Displays real-time temperature and weather conditions</li>
  <li>💨 Shows wind speed</li>
  <li>🌫️ Fetches Air Quality Index (AQI)</li>
  <li>📍 Converts city names to coordinates using geocoding</li>
  <li>⚡ Lightweight terminal-based interface</li>
</ul>

<hr>

<h2>Sample Output</h2>

<pre>
========================================
  Mumbai, India
========================================
  Temperature:    30.2 C
  Condition:      Clear sky
  Wind Speed:     12.5 km/h
  Air Quality:    85 - Moderate
========================================
</pre>

<hr>

<h2>Installation & Usage</h2>

<h3>1. Clone the repository</h3>
<pre>
git clone https://github.com/ogkrrish/c-live-weather-and-aqi-fetcher.git
cd c-live-weather-and-aqi-fetcher
</pre>

<h3>2. Compile the program</h3>
<pre>
gcc main.c -o weather
</pre>

<h3>3. Run it</h3>
<pre>
./weather
</pre>

<h3>4. Enter a city name</h3>
<pre>
Enter city name (or 'quit' to exit): Mumbai
</pre>

<hr>

<h2>Requirements</h2>

<ul>
  <li>GCC compiler</li>
  <li><code>curl</code> installed on your system</li>
  <li>Internet connection</li>
</ul>

<hr>

<h2>How It Works</h2>

<ol>
  <li>The user enters a city name.</li>
  <li>The app uses geocoding to convert the city into latitude and longitude.</li>
  <li>It fetches weather data and AQI data from Open-Meteo APIs.</li>
  <li>The JSON response is parsed manually.</li>
  <li>The results are displayed in the terminal in a readable format.</li>
</ol>

<hr>

<h2>Limitations</h2>

<ul>
  <li>Uses <code>system("curl")</code> instead of a native HTTP library</li>
  <li>Manual JSON parsing is fragile if API response format changes</li>
  <li>Basic error handling only</li>
  <li>Creates a temporary file during API fetch</li>
</ul>

<hr>

<h2>Future Improvements</h2>

<ul>
  <li>Replace system calls with <b>libcurl</b></li>
  <li>Use a proper JSON parsing library</li>
  <li>Add forecast support</li>
  <li>Improve error handling and logging</li>
  <li>Build a GUI or web version in the future</li>
</ul>

<hr>

<h2>📁 Project Structure</h2>

<pre>
c-live-weather-and-aqi-fetcher/
├── main.c
├── README.md
└── .gitignore
└── .gitattributes
└── LICENCE
</pre>

<hr>

<h2>Author</h2>

<p>
  <b>Krish Rupawat</b></a><br>
  ogkrrish<br>
</p>

<hr>

<h2>Support</h2>

<p>If you found this project useful:</p>

<ul>
  <li>⭐ Star the repository</li>
  <li>🍴 Fork it</li>
  <li>🧠 Suggest improvements</li>
</ul>
<p align="center">
  <a href="https://github.com/ogkrrish">
    <img src="https://img.shields.io/badge/Follow%20on-GitHub-black?style=for-the-badge&logo=github">
  </a>
  
  <a href="https://www.linkedin.com/in/krish-rupawat/">
    <img src="https://img.shields.io/badge/Connect%20on-LinkedIn-blue?style=for-the-badge&logo=linkedin">
  </a>
</p>
<hr>

<h2>License</h2>

<p>
This project is open-source and available under the <b>MIT License</b>.
</p>
