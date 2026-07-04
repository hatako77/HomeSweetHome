#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fa" dir="rtl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        .container {
            background: white;
            border-radius: 20px;
            padding: 50px;
            text-align: center;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            max-width: 500px;
            width: 100%;
        }
        h1 {
            color: #333;
            font-size: 2.5rem;
            margin-bottom: 10px;
        }
        .version {
            color: #667eea;
            font-size: 1.2rem;
            font-weight: bold;
            margin: 15px 0;
        }
        .status {
            color: #48bb78;
            font-size: 1rem;
        }
        .chip {
            display: inline-block;
            background: #f0f0f0;
            padding: 5px 15px;
            border-radius: 50px;
            font-size: 0.8rem;
            color: #666;
            margin-top: 10px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>👋 سلام</h1>
        <p style="color:#666;font-size:1.1rem;">ESP32 با موفقیت راه‌اندازی شد!</p>
        <div class="version">نسخه: <span id="version">---</span></div>
        <div class="status">✅ دستگاه آنلاین است</div>
        <div class="chip">ESP32</div>
    </div>

    <script>
        fetch('/api/version')
            .then(res => res.json())
            .then(data => {
                document.getElementById('version').textContent = data.version || '---';
            })
            .catch(() => {
                document.getElementById('version').textContent = 'Error';
            });
    </script>
</body>
</html>
)rawliteral";

#endif
