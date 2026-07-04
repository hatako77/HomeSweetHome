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
            max-width: 600px;
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
        .btn {
            display: inline-block;
            background: #667eea;
            color: white;
            padding: 12px 30px;
            border-radius: 50px;
            text-decoration: none;
            font-weight: bold;
            margin-top: 20px;
            transition: all 0.3s ease;
        }
        .btn:hover {
            background: #5a67d8;
            transform: translateY(-2px);
            box-shadow: 0 5px 20px rgba(102, 126, 234, 0.4);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>👋 سسس</h1>
        <p style="color:#666;font-size:1.1rem;">ESP32 با موفقیت راه‌اندازی شد!</p>
        <div class="version">نسخه: <span id="version">---</span></div>
        <div class="status">✅ دستگاه آنلاین است</div>
        <div class="chip">ESP32</div>
        <a href="/ota" class="btn">🔄 به‌روزرسانی OTA</a>
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

const char OTA_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fa" dir="rtl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>به‌روزرسانی OTA</title>
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
            padding: 40px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            max-width: 600px;
            width: 100%;
        }
        h1 {
            color: #333;
            text-align: center;
            margin-bottom: 30px;
        }
        .card {
            background: #f8f9fa;
            border-radius: 15px;
            padding: 20px;
            margin-bottom: 20px;
            border: 1px solid #e9ecef;
        }
        .card h2 {
            color: #495057;
            font-size: 1.1rem;
            margin-bottom: 15px;
        }
        .info-row {
            display: flex;
            justify-content: space-between;
            padding: 8px 0;
            border-bottom: 1px solid #e9ecef;
        }
        .info-row:last-child {
            border-bottom: none;
        }
        .label {
            color: #6c757d;
        }
        .value {
            font-weight: bold;
            color: #333;
        }
        .value.latest {
            color: #667eea;
        }
        .value.available {
            color: #48bb78;
        }
        .btn {
            display: inline-block;
            background: #667eea;
            color: white;
            padding: 10px 25px;
            border: none;
            border-radius: 50px;
            cursor: pointer;
            font-size: 0.95rem;
            font-weight: bold;
            transition: all 0.3s ease;
            width: 100%;
            margin-top: 10px;
        }
        .btn:hover {
            background: #5a67d8;
            transform: translateY(-2px);
            box-shadow: 0 5px 20px rgba(102, 126, 234, 0.4);
        }
        .btn:disabled {
            opacity: 0.5;
            cursor: not-allowed;
            transform: none;
        }
        .btn-success {
            background: #48bb78;
        }
        .btn-success:hover {
            background: #38a169;
        }
        .btn-danger {
            background: #fc8181;
        }
        .btn-danger:hover {
            background: #f56565;
        }
        .btn-back {
            background: #6c757d;
            margin-bottom: 10px;
        }
        .btn-back:hover {
            background: #5a6268;
        }
        .progress-container {
            margin: 15px 0;
        }
        .progress {
            width: 100%;
            height: 25px;
            background: #e9ecef;
            border-radius: 50px;
            overflow: hidden;
            position: relative;
        }
        .progress-bar {
            height: 100%;
            background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
            width: 0%;
            transition: width 0.5s ease;
            border-radius: 50px;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .progress-text {
            color: white;
            font-size: 0.8rem;
            font-weight: bold;
            position: absolute;
            left: 50%;
            top: 50%;
            transform: translate(-50%, -50%);
            z-index: 1;
        }
        .status-box {
            background: white;
            border: 1px solid #dee2e6;
            border-radius: 10px;
            padding: 10px 15px;
            margin-top: 10px;
            text-align: center;
            color: #495057;
            min-height: 40px;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .status-box .spinner {
            display: inline-block;
            width: 20px;
            height: 20px;
            border: 3px solid #f3f3f3;
            border-top: 3px solid #667eea;
            border-radius: 50%;
            animation: spin 1s linear infinite;
            margin-right: 10px;
        }
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
        .hidden {
            display: none;
        }
        .success-message {
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
            border-radius: 10px;
            padding: 15px;
            text-align: center;
            margin-top: 15px;
            animation: fadeIn 0.5s ease;
        }
        @keyframes fadeIn {
            from { opacity: 0; transform: scale(0.9); }
            to { opacity: 1; transform: scale(1); }
        }
        @media (max-width: 600px) {
            .container { padding: 20px; }
        }
    </style>
</head>
<body>
    <div class="container">
        <button class="btn btn-back" onclick="window.location.href='/'">← بازگشت به صفحه اصلی</button>
        
        <h1>🔄 به‌روزرسانی OTA</h1>
        
        <div class="card">
            <h2>📊 وضعیت نسخه</h2>
            <div class="info-row">
                <span class="label">نسخه فعلی</span>
                <span class="value" id="currentVersion">---</span>
            </div>
            <div class="info-row">
                <span class="label">آخرین نسخه</span>
                <span class="value latest" id="latestVersion">---</span>
            </div>
            <div class="info-row">
                <span class="label">وضعیت</span>
                <span class="value available" id="updateStatus">در حال بررسی...</span>
            </div>
        </div>
        
        <div class="card">
            <h2>⬇️ به‌روزرسانی خودکار</h2>
            <button class="btn" id="checkBtn" onclick="checkUpdate()">🔍 بررسی نسخه جدید</button>
            <button class="btn btn-success hidden" id="updateBtn" onclick="startUpdate()">⬇️ دانلود و نصب</button>
            
            <div class="progress-container">
                <div class="progress">
                    <div class="progress-bar" id="progressBar" style="width: 0%">
                        <span class="progress-text" id="progressText">۰%</span>
                    </div>
                </div>
            </div>
            
            <div class="status-box" id="statusBox">
                <span id="statusText">آماده به‌روزرسانی</span>
            </div>
            
            <div id="successMessage" class="success-message hidden">
                ✅ به‌روزرسانی با موفقیت انجام شد!
            </div>
        </div>
    </div>

    <script>
        let eventSource = null;
        let updateInProgress = false;

        function connectEventSource() {
            if (eventSource) eventSource.close();
            eventSource = new EventSource('/events');
            eventSource.onmessage = function(e) {
                try {
                    const data = JSON.parse(e.data);
                    if (data.type === 'progress') {
                        updateProgress(data.progress, data.status);
                    } else if (data.type === 'complete') {
                        updateProgress(100, '✅ ' + data.message);
                        document.getElementById('successMessage').classList.remove('hidden');
                        setTimeout(() => {
                            window.location.href = '/';
                        }, 3000);
                    }
                } catch(err) {
                    console.error('Error parsing event:', err);
                }
            };
        }

        function updateProgress(progress, status) {
            const bar = document.getElementById('progressBar');
            const text = document.getElementById('progressText');
            const statusText = document.getElementById('statusText');
            
            bar.style.width = Math.min(progress, 100) + '%';
            text.textContent = Math.min(progress, 100) + '%';
            
            if (status) {
                statusText.innerHTML = (progress > 0 && progress < 100) ? 
                    '<span class="spinner"></span> ' + status : 
                    status;
            }
            
            if (progress > 0 && progress < 100) {
                updateInProgress = true;
                document.getElementById('checkBtn').disabled = true;
                document.getElementById('updateBtn').disabled = true;
            } else {
                updateInProgress = false;
                document.getElementById('checkBtn').disabled = false;
                document.getElementById('updateBtn').disabled = false;
            }
        }

        function checkUpdate() {
            if (updateInProgress) return;
            
            const statusText = document.getElementById('statusText');
            statusText.innerHTML = '<span class="spinner"></span> در حال بررسی نسخه...';
            document.getElementById('checkBtn').disabled = true;
            
            fetch('/api/check-update')
                .then(res => res.json())
                .then(data => {
                    document.getElementById('currentVersion').textContent = data.current || '---';
                    document.getElementById('latestVersion').textContent = data.latest || '---';
                    
                    if (data.updateAvailable) {
                        document.getElementById('updateStatus').textContent = '✅ نسخه جدید موجود است!';
                        document.getElementById('updateStatus').style.color = '#48bb78';
                        document.getElementById('updateBtn').classList.remove('hidden');
                        statusText.innerHTML = 'نسخه جدید موجود است! روی دکمه زیر کلیک کنید.';
                    } else {
                        document.getElementById('updateStatus').textContent = '✅ دستگاه به‌روز است';
                        document.getElementById('updateStatus').style.color = '#48bb78';
                        document.getElementById('updateBtn').classList.add('hidden');
                        statusText.innerHTML = 'دستگاه به‌روز است.';
                    }
                    document.getElementById('checkBtn').disabled = false;
                })
                .catch(error => {
                    statusText.innerHTML = '❌ خطا: ' + error;
                    document.getElementById('checkBtn').disabled = false;
                });
        }

        function startUpdate() {
            if (updateInProgress) return;
            
            if (!confirm('آیا از به‌روزرسانی دستگاه اطمینان دارید؟')) return;
            
            const statusText = document.getElementById('statusText');
            statusText.innerHTML = '<span class="spinner"></span> شروع به‌روزرسانی...';
            document.getElementById('updateBtn').disabled = true;
            document.getElementById('checkBtn').disabled = true;
            
            fetch('/api/start-update', { method: 'POST' })
                .then(res => res.json())
                .then(data => {
                    if (data.success) {
                        statusText.innerHTML = '✅ به‌روزرسانی شروع شد!';
                    } else {
                        statusText.innerHTML = '❌ خطا: ' + (data.error || 'Unknown error');
                        document.getElementById('updateBtn').disabled = false;
                        document.getElementById('checkBtn').disabled = false;
                    }
                })
                .catch(error => {
                    statusText.innerHTML = '❌ خطا: ' + error;
                    document.getElementById('updateBtn').disabled = false;
                    document.getElementById('checkBtn').disabled = false;
                });
        }

        // Initialize
        document.addEventListener('DOMContentLoaded', function() {
            connectEventSource();
            // Get current version
            fetch('/api/version')
                .then(res => res.json())
                .then(data => {
                    document.getElementById('currentVersion').textContent = data.version || '---';
                });
        });
    </script>
</body>
</html>
)rawliteral";

#endif
