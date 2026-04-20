# c-live-weather-and-aqi-fetcher
A command-line weather application written in C that fetches real-time weather and air quality data using Open-Meteo APIs. Includes geocoding, manual JSON parsing, and system-level HTTP requests using curl.
<h1 align="center">🌦️ C Live Weather & AQI Fetcher</h1>

<p align="center">
  <b>A real-time Weather + Air Quality CLI app built in pure C</b><br>
  <i>Because GUIs are overrated and suffering builds character</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue?style=for-the-badge">
  <img src="https://img.shields.io/badge/API-OpenMeteo-green?style=for-the-badge">
  <img src="https://img.shields.io/badge/Platform-CLI-black?style=for-the-badge">
  <img src="https://img.shields.io/badge/Status-Working-success?style=for-the-badge">
</p>

---

## ✨ Features

- 🌍 Search weather by **city name**
- 🌡️ Real-time **temperature & conditions**
- 💨 Wind speed tracking
- 🌫️ **Air Quality Index (AQI)** with category
- 📍 Built-in **geocoding (city → coordinates)**
- 🧠 Custom JSON parsing (no libraries, only pain)

---

## 🖥️ Demo

```bash
========================================
  Mumbai, India
========================================
Temperature:    30.2 C
Condition:      Clear sky
Wind Speed:     12.5 km/h
Air Quality:    85 - Moderate
========================================
