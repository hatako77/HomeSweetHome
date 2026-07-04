#include "web_server.h"
#include "version.h"
#include "config.h"
#include "ota_handler.h"
#include <ArduinoJson.h>

extern OTAHandler otaHandler;

// ============================================
// تمام فایل‌های HTML با PROGMEM
// ============================================

// ===== index.html =====
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fa" dir="rtl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>دستگاه ESP32</title>
    <style>
        *{margin:0;padding:0;box-sizing:border-box}
        :root{--primary:#667eea;--primary-dark:#5a67d8;--secondary:#764ba2;--success:#48bb78;--warning:#ed8936;--danger:#fc8181;--info:#63b3ed;--gray-100:#f7fafc;--gray-200:#edf2f7;--gray-300:#e2e8f0;--gray-500:#a0aec0;--gray-700:#4a5568;--gray-900:#1a202c;--shadow:0 10px 40px rgba(0,0,0,0.15);--radius:16px;--transition:all 0.3s cubic-bezier(0.4,0,0.2,1)}
        body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,'Helvetica Neue',Arial,sans-serif;background:linear-gradient(135deg,var(--primary)0%,var(--secondary)100%);min-height:100vh;padding:20px;display:flex;justify-content:center;align-items:center}
        .container{max-width:820px;width:100%;background:white;border-radius:var(--radius);padding:35px;box-shadow:var(--shadow);animation:slideUp 0.6s ease-out}
        @keyframes slideUp{from{opacity:0;transform:translateY(30px) scale(0.98)}to{opacity:1;transform:translateY(0) scale(1)}}
        header{display:flex;justify-content:space-between;align-items:center;margin-bottom:30px;flex-wrap:wrap;gap:15px}
        header h1{color:var(--gray-900);font-size:1.8rem;font-weight:600;letter-spacing:-0.5px}
        .status-indicator{display:flex;align-items:center;gap:8px;padding:8px 16px;background:var(--gray-100);border-radius:50px;font-size:0.9rem;color:var(--gray-700)}
        .dot{width:10px;height:10px;border-radius:50%;display:inline-block}
        .dot.online{background:var(--success);animation:pulse 2s infinite}
        @keyframes pulse{0%,100%{opacity:1}50%{opacity:0.5}}
        .card{background:var(--gray-100);border-radius:var(--radius);padding:25px;margin-bottom:25px;border:1px solid var(--gray-200);transition:var(--transition)}
        .card:hover{box-shadow:0 4px 12px rgba(0,0,0,0.05)}
        .card h2{color:var(--gray-700);font-size:1.1rem;font-weight:600;margin-bottom:18px;display:flex;align-items:center;gap:10px}
        .info-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:15px}
        .info-item{display:flex;flex-direction:column;gap:4px}
        .info-item .label{font-size:0.8rem;color:var(--gray-500);text-transform:uppercase;letter-spacing:0.5px;font-weight:600}
        .info-item .value{font-size:1.1rem;font-weight:500;color:var(--gray-900)}
        .btn{display:inline-flex;align-items:center;gap:8px;padding:10px 24px;border:none;border-radius:50px;cursor:pointer;font-size:0.95rem;font-weight:500;text-decoration:none;transition:var(--transition);color:white}
        .btn:hover{transform:translateY(-2px);box-shadow:0 8px 25px rgba(0,0,0,0.15)}
        .btn:disabled{opacity:0.5;cursor:not-allowed;transform:none!important}
        .btn-primary{background:var(--primary)}.btn-primary:hover{background:var(--primary-dark)}
        .btn-success{background:var(--success)}.btn-success:hover{background:#38a169}
        .btn-warning{background:var(--warning)}.btn-warning:hover{background:#dd6b20}
        .btn-info{background:var(--info)}.btn-info:hover{background:#4299e1}
        .btn-group{display:flex;flex-wrap:wrap;gap:10px}
        .btn-group .btn{flex:1;min-width:140px;justify-content:center}
        footer{text-align:center;color:var(--gray-500);font-size:0.85rem;margin-top:10px;padding-top:20px;border-top:1px solid var(--gray-200)}
        @media(max-width:600px){.container{padding:20px}header{flex-direction:column;text-align:center}.info-grid{grid-template-columns:1fr 1fr}.btn-group .btn{flex:1 1 100%}}
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>🎛️ پنل کنترل ESP32</h1>
            <div class="status-indicator">
                <span class="dot online"></span>
                <span>آنلاین</span>
            </div>
        </header>
        
        <div class="card">
            <h2>📊 اطلاعات دستگاه</h2>
            <div class="info-grid">
                <div class="info-item">
                    <span class="label">نسخه فریم‌ور</span>
                    <span class="value" id="version">---</span>
                </div>
                <div class="info-item">
                    <span class="label">زمان فعالیت</span>
                    <span class="value" id="uptime">---</span>
                </div>
                <div class="info-item">
                    <span class="label">آدرس IP</span>
                    <span class="value" id="ip">---</span>
                </div>
                <div class="info-item">
                    <span class="label">قدرت سیگنال</span>
                    <span class="value" id="signal">---</span>
                </div>
            </div>
        </div>
        
        <div class="card">
            <h2>⚡ اقدامات سریع</h2>
            <div class="btn-group">
                <a href="/ota" class="btn btn-primary">🔄 به‌روزرسانی OTA</a>
                <button class="btn btn-warning" onclick="restartDevice()">🔁 راه‌اندازی مجدد</button>
            </div>
        </div>
        
        <footer>
            <p>ESP32 OTA System v<span id="footerVersion">---</span></p>
        </footer>
    </div>
    
    <script>
        // ===== Main.js =====
        let startTime = Date.now();
        setInterval(() => {
            let uptime = Math.floor((Date.now() - startTime) / 1000);
            let hours = Math.floor(uptime / 3600);
            let minutes = Math.floor((uptime % 3600) / 60);
            let seconds = uptime % 60;
            document.getElementById('uptime').textContent = 
                hours + 'h ' + minutes + 'm ' + seconds + 's';
        }, 1000);

        fetch('/api/version')
            .then(response => response.json())
            .then(data => {
                document.getElementById('version').textContent = data.current || '---';
                document.getElementById('footerVersion').textContent = data.current || '---';
            })
            .catch(() => {
                document.getElementById('version').textContent = 'Error';
            });

        fetch('/api/ip')
            .then(response => response.json())
            .then(data => {
                document.getElementById('ip').textContent = data.ip || '---';
            })
            .catch(() => {
                document.getElementById('ip').textContent = '---';
            });

        function restartDevice() {
            if (confirm('آیا از راه‌اندازی مجدد دستگاه اطمینان دارید؟')) {
                const btn = document.querySelector('.btn-warning');
                btn.disabled = true;
                btn.innerHTML = '⏳ در حال راه‌اندازی...';
                fetch('/api/restart', { method: 'POST' })
                    .then(() => {
                        alert('✅ دستگاه در حال راه‌اندازی مجدد...');
                        setTimeout(() => window.location.reload(), 5000);
                    })
                    .catch(() => {
                        alert('❌ خطا در راه‌اندازی مجدد');
                        btn.disabled = false;
                        btn.innerHTML = '🔁 راه‌اندازی مجدد';
                    });
            }
        }

        // Signal strength simulation
        setInterval(() => {
            const signal = Math.floor(60 + Math.random() * 40);
            document.getElementById('signal').textContent = signal + '%';
        }, 5000);
    </script>
</body>
</html>
)rawliteral";

// ===== ota.html =====
const char OTA_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fa" dir="rtl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>به‌روزرسانی OTA</title>
    <style>
        *{margin:0;padding:0;box-sizing:border-box}
        :root{--primary:#667eea;--primary-dark:#5a67d8;--secondary:#764ba2;--success:#48bb78;--warning:#ed8936;--danger:#fc8181;--info:#63b3ed;--gray-100:#f7fafc;--gray-200:#edf2f7;--gray-300:#e2e8f0;--gray-500:#a0aec0;--gray-700:#4a5568;--gray-900:#1a202c;--shadow:0 10px 40px rgba(0,0,0,0.15);--radius:16px;--transition:all 0.3s cubic-bezier(0.4,0,0.2,1)}
        body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,'Helvetica Neue',Arial,sans-serif;background:linear-gradient(135deg,var(--primary)0%,var(--secondary)100%);min-height:100vh;padding:20px;display:flex;justify-content:center;align-items:center}
        .container{max-width:820px;width:100%;background:white;border-radius:var(--radius);padding:35px;box-shadow:var(--shadow);animation:slideUp 0.6s ease-out}
        @keyframes slideUp{from{opacity:0;transform:translateY(30px) scale(0.98)}to{opacity:1;transform:translateY(0) scale(1)}}
        .page-header{display:flex;justify-content:space-between;align-items:center;margin-bottom:30px;flex-wrap:wrap;gap:15px}
        .page-header h1{color:var(--gray-900);font-size:1.6rem;font-weight:600}
        .btn-back{background:var(--gray-200);color:var(--gray-700);padding:8px 20px;border-radius:50px;text-decoration:none;transition:var(--transition);font-weight:500}
        .btn-back:hover{background:var(--gray-300);transform:translateX(-3px)}
        .info-card{background:linear-gradient(135deg,var(--primary)0%,var(--secondary)100%);border-radius:var(--radius);padding:25px;color:white;margin-bottom:25px;display:flex;justify-content:space-between;align-items:center;flex-wrap:wrap;gap:15px}
        .version-display{display:flex;align-items:center;gap:15px}
        .version-display .label{font-size:0.9rem;opacity:0.8;font-weight:500}
        .version-badge{background:rgba(255,255,255,0.2);padding:8px 20px;border-radius:50px;font-size:1.2rem;font-weight:600;backdrop-filter:blur(10px);font-family:monospace}
        .device-info{display:flex;align-items:center;gap:12px}
        .device-info .chip{background:rgba(255,255,255,0.15);padding:4px 12px;border-radius:50px;font-size:0.8rem}
        .device-info .status{font-size:0.9rem}
        .card{background:var(--gray-100);border-radius:var(--radius);padding:25px;margin-bottom:25px;border:1px solid var(--gray-200);transition:var(--transition)}
        .card:hover{box-shadow:0 4px 12px rgba(0,0,0,0.05)}
        .card h2{color:var(--gray-700);font-size:1.1rem;font-weight:600;margin-bottom:18px}
        .subtitle{color:var(--gray-500);font-size:0.9rem;margin-bottom:15px}
        .version-compare{display:flex;justify-content:center;align-items:center;gap:20px;padding:15px;background:white;border-radius:var(--radius);border:1px solid var(--gray-200);margin-bottom:18px}
        .version-item{display:flex;flex-direction:column;align-items:center;gap:4px}
        .version-item .label{font-size:0.75rem;color:var(--gray-500);text-transform:uppercase;font-weight:600;letter-spacing:0.5px}
        .version-item .value{font-size:1.1rem;font-weight:500;color:var(--gray-700);font-family:monospace}
        .version-item .value.highlight{color:var(--primary);font-weight:700}
        .version-arrow{font-size:1.5rem;color:var(--gray-300)}
        .btn{display:inline-flex;align-items:center;gap:8px;padding:10px 24px;border:none;border-radius:50px;cursor:pointer;font-size:0.95rem;font-weight:500;text-decoration:none;transition:var(--transition);color:white}
        .btn:hover{transform:translateY(-2px);box-shadow:0 8px 25px rgba(0,0,0,0.15)}
        .btn:disabled{opacity:0.5;cursor:not-allowed;transform:none!important}
        .btn-primary{background:var(--primary)}.btn-primary:hover{background:var(--primary-dark)}
        .btn-success{background:var(--success)}.btn-success:hover{background:#38a169}
        .btn-info{background:var(--info)}.btn-info:hover{background:#4299e1}
        .btn-group{display:flex;flex-wrap:wrap;gap:10px;margin-bottom:15px}
        .hidden{display:none!important}
        .progress-container{margin:15px 0}
        .progress{width:100%;height:32px;background:var(--gray-200);border-radius:50px;overflow:hidden;position:relative}
        .progress-bar{height:100%;background:linear-gradient(90deg,var(--primary)0%,var(--secondary)100%);width:0%;transition:width 0.5s ease;border-radius:50px;display:flex;align-items:center;justify-content:center}
        .progress-text{color:white;font-size:0.85rem;font-weight:600;text-shadow:0 1px 3px rgba(0,0,0,0.3);position:absolute;left:50%;top:50%;transform:translate(-50%,-50%);z-index:1}
        .status-box{background:white;border:1px solid var(--gray-200);border-radius:var(--radius);padding:12px 16px;margin-top:10px;display:flex;align-items:center;gap:12px;min-height:48px}
        .status-box .icon{font-size:1.2rem}
        .status-box .status-text{color:var(--gray-700);font-size:0.9rem}
        .status-box .spinner{display:inline-block;width:18px;height:18px;border:3px solid var(--gray-200);border-top:3px solid var(--primary);border-radius:50%;animation:spin 0.8s linear infinite}
        @keyframes spin{0%{transform:rotate(0deg)}100%{transform:rotate(360deg)}}
        .file-input-wrapper{margin:15px 0}
        .file-input{display:flex;align-items:center;gap:10px;padding:14px 20px;background:white;border:2px dashed var(--gray-300);border-radius:var(--radius);cursor:pointer;transition:var(--transition);width:100%}
        .file-input:hover{border-color:var(--primary);background:#f8f9ff}
        .file-input input[type='file']{display:none}
        .file-name{color:var(--gray-500);font-size:0.9rem;margin-left:auto}
        .success-overlay{position:fixed;top:0;left:0;right:0;bottom:0;background:rgba(0,0,0,0.6);backdrop-filter:blur(10px);display:flex;justify-content:center;align-items:center;z-index:1000;animation:fadeIn 0.5s ease}
        .success-overlay.hidden{display:none}
        .success-content{background:white;border-radius:var(--radius);padding:50px;text-align:center;max-width:450px;width:90%;box-shadow:0 30px 80px rgba(0,0,0,0.3);animation:popIn 0.5s ease}
        @keyframes fadeIn{from{opacity:0}to{opacity:1}}
        @keyframes popIn{from{opacity:0;transform:scale(0.8)}to{opacity:1;transform:scale(1)}}
        .success-icon{font-size:4rem;margin-bottom:15px}
        .success-content h2{color:var(--gray-900);font-size:1.4rem;margin-bottom:10px}
        .success-content p{color:var(--gray-500);margin-bottom:25px}
        .countdown-circle{position:relative;width:80px;height:80px;margin:0 auto}
        .countdown-circle svg{transform:rotate(-90deg);width:100%;height:100%}
        .countdown-circle .bg{fill:none;stroke:var(--gray-200);stroke-width:6}
        .countdown-circle .progress-ring{fill:none;stroke:var(--success);stroke-width:6;stroke-linecap:round;stroke-dasharray:283;stroke-dashoffset:0;transition:stroke-dashoffset 1s linear}
        .countdown-text{position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);font-size:2rem;font-weight:700;color:var(--gray-900)}
        @media(max-width:600px){.container{padding:20px}.page-header{flex-direction:column;text-align:center}.info-card{flex-direction:column;text-align:center}.version-compare{flex-direction:column;gap:10px}.version-arrow{transform:rotate(90deg)}.btn-group .btn{flex:1 1 100%}.success-content{padding:30px}}
    </style>
</head>
<body>
    <div class="container">
        <header class="page-header">
            <h1>🔄 به‌روزرسانی سیستمی</h1>
            <a href="/" class="btn-back">← بازگشت</a>
        </header>
        
        <div class="info-card">
            <div class="version-display">
                <span class="label">نسخه فعلی</span>
                <span class="version-badge" id="currentVersion">---</span>
            </div>
            <div class="device-info">
                <span class="chip">ESP32</span>
                <span class="status" id="deviceStatus">🟢 فعال</span>
            </div>
        </div>
        
        <!-- Automatic Update -->
        <div class="card">
            <h2>🤖 به‌روزرسانی خودکار</h2>
            <div class="version-compare">
                <div class="version-item">
                    <span class="label">نسخه نصب شده</span>
                    <span class="value" id="installedVersion">---</span>
                </div>
                <div class="version-arrow">→</div>
                <div class="version-item">
                    <span class="label">نسخه جدید</span>
                    <span class="value highlight" id="latestVersion">---</span>
                </div>
            </div>
            
            <div class="btn-group">
                <button class="btn btn-info" onclick="checkUpdate()" id="checkBtn">
                    🔍 بررسی نسخه جدید
                </button>
                <button class="btn btn-success hidden" onclick="startUpdate()" id="updateBtn">
                    ⬇️ دانلود و نصب
                </button>
            </div>
            
            <div class="progress-container">
                <div class="progress">
                    <div class="progress-bar" id="autoProgress" style="width: 0%">
                        <span class="progress-text">۰%</span>
                    </div>
                </div>
            </div>
            
            <div class="status-box" id="autoStatus">
                <span class="icon">⏳</span>
                <span class="status-text">آماده به‌روزرسانی</span>
            </div>
        </div>
        
        <!-- Manual Update -->
        <div class="card">
            <h2>📁 به‌روزرسانی دستی</h2>
            <p class="subtitle">فایل فریم‌ور (firmware.bin) را انتخاب کنید</p>
            
            <form id="uploadForm" enctype="multipart/form-data">
                <div class="file-input-wrapper">
                    <label class="file-input">
                        <span class="icon">📂</span>
                        <span>انتخاب فایل</span>
                        <input type="file" name="firmware" accept=".bin" required>
                        <span class="file-name" id="fileName">هیچ فایلی انتخاب نشده</span>
                    </label>
                </div>
                
                <button type="submit" class="btn btn-primary" id="manualUploadBtn">
                    ⬆️ آپلود و نصب
                </button>
            </form>
            
            <div class="progress-container">
                <div class="progress">
                    <div class="progress-bar" id="manualProgress" style="width: 0%">
                        <span class="progress-text">۰%</span>
                    </div>
                </div>
            </div>
            
            <div class="status-box" id="manualStatus">
                <span class="icon">📤</span>
                <span class="status-text">آماده آپلود</span>
            </div>
        </div>
        
        <!-- Success Overlay -->
        <div id="successMessage" class="success-overlay hidden">
            <div class="success-content">
                <div class="success-icon">✅</div>
                <h2>به‌روزرسانی با موفقیت انجام شد!</h2>
                <p>دستگاه در حال راه‌اندازی مجدد...</p>
                <div class="countdown-circle">
                    <svg viewBox="0 0 100 100">
                        <circle cx="50" cy="50" r="45" class="bg"/>
                        <circle cx="50" cy="50" r="45" class="progress-ring" id="countdownRing"/>
                    </svg>
                    <span class="countdown-text" id="countdownText">۵</span>
                </div>
            </div>
        </div>
    </div>
    
    <script>
        // ===== ota.js =====
        let eventSource = null;
        let redirectTimer = null;
        let countdown = 5;
        let updateInProgress = false;

        function connectEventSource() {
            if (eventSource) eventSource.close();
            eventSource = new EventSource('/events');
            eventSource.onmessage = function(e) {
                try {
                    const data = JSON.parse(e.data);
                    handleEvent(data);
                } catch(err) {
                    console.error('Error parsing event:', err);
                }
            };
            eventSource.onerror = function(e) {
                console.error('EventSource error:', e);
                if (e.target.readyState === EventSource.CLOSED) {
                    setTimeout(connectEventSource, 5000);
                }
            };
        }

        function handleEvent(data) {
            const { type, progress, status, message } = data;
            if (type === 'progress') {
                updateProgress(progress, status);
            } else if (type === 'complete') {
                updateProgress(100, '✅ ' + (message || 'Update complete!'));
                showSuccessMessage(message || 'Update completed successfully!');
                startRedirectTimer();
            }
        }

        function updateProgress(progress, status) {
            const autoProgress = document.getElementById('autoProgress');
            const autoStatus = document.getElementById('autoStatus');
            if (autoProgress) {
                autoProgress.style.width = Math.min(progress, 100) + '%';
                autoProgress.querySelector('.progress-text').textContent = Math.min(progress, 100) + '%';
            }
            if (autoStatus) {
                autoStatus.querySelector('.status-text').textContent = status;
                autoStatus.querySelector('.icon').innerHTML = (progress > 0 && progress < 100) ? 
                    '<span class="spinner"></span>' : 
                    (progress >= 100 ? '✅' : '⏳');
            }
            
            const manualProgress = document.getElementById('manualProgress');
            const manualStatus = document.getElementById('manualStatus');
            if (manualProgress) {
                manualProgress.style.width = Math.min(progress, 100) + '%';
                manualProgress.querySelector('.progress-text').textContent = Math.min(progress, 100) + '%';
            }
            if (manualStatus) {
                manualStatus.querySelector('.status-text').textContent = status;
                manualStatus.querySelector('.icon').innerHTML = (progress > 0 && progress < 100) ? 
                    '<span class="spinner"></span>' : 
                    (progress >= 100 ? '✅' : '📤');
            }
            
            updateInProgress = (progress > 0 && progress < 100);
            document.querySelectorAll('.btn').forEach(btn => btn.disabled = updateInProgress);
        }

        function checkUpdate() {
            if (updateInProgress) return;
            const btn = document.getElementById('checkBtn');
            const status = document.getElementById('autoStatus');
            btn.disabled = true;
            status.querySelector('.status-text').textContent = 'در حال بررسی نسخه جدید...';
            status.querySelector('.icon').innerHTML = '<span class="spinner"></span>';
            
            fetch('/api/version')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('latestVersion').textContent = data.latest || '---';
                    document.getElementById('installedVersion').textContent = data.current || '---';
                    if (data.updateAvailable) {
                        status.querySelector('.status-text').textContent = '✅ نسخه جدید موجود است!';
                        status.querySelector('.icon').textContent = '🎉';
                        document.getElementById('updateBtn').classList.remove('hidden');
                        document.getElementById('updateBtn').disabled = false;
                        document.getElementById('deviceStatus').textContent = '🔄 به‌روزرسانی موجود';
                    } else {
                        status.querySelector('.status-text').textContent = '✅ دستگاه به‌روز است';
                        status.querySelector('.icon').textContent = '✅';
                        document.getElementById('updateBtn').classList.add('hidden');
                        document.getElementById('deviceStatus').textContent = '🟢 فعال';
                    }
                    btn.disabled = false;
                })
                .catch(error => {
                    status.querySelector('.status-text').textContent = '❌ خطا: ' + error.message;
                    status.querySelector('.icon').textContent = '❌';
                    btn.disabled = false;
                });
        }

        function startUpdate() {
            if (updateInProgress) return;
            const btn = document.getElementById('updateBtn');
            const status = document.getElementById('autoStatus');
            btn.disabled = true;
            status.querySelector('.status-text').textContent = 'در حال شروع به‌روزرسانی...';
            status.querySelector('.icon').innerHTML = '<span class="spinner"></span>';
            
            fetch('/api/update', { method: 'POST' })
                .then(response => response.json())
                .then(data => {
                    if (data.success) {
                        status.querySelector('.status-text').textContent = '✅ به‌روزرسانی شروع شد!';
                        status.querySelector('.icon').textContent = '🚀';
                    } else {
                        status.querySelector('.status-text').textContent = '❌ خطا: ' + (data.error || 'Unknown error');
                        status.querySelector('.icon').textContent = '❌';
                        btn.disabled = false;
                    }
                })
                .catch(error => {
                    status.querySelector('.status-text').textContent = '❌ خطا: ' + error.message;
                    status.querySelector('.icon').textContent = '❌';
                    btn.disabled = false;
                });
        }

        document.getElementById('uploadForm').onsubmit = function(e) {
            e.preventDefault();
            if (updateInProgress) return;
            const fileInput = this.querySelector('input[type="file"]');
            const file = fileInput.files[0];
            if (!file) { alert('لطفاً یک فایل انتخاب کنید'); return; }
            if (!file.name.endsWith('.bin')) { alert('لطفاً فایل با پسوند .bin انتخاب کنید'); return; }
            
            const formData = new FormData();
            formData.append('firmware', file);
            const btn = document.getElementById('manualUploadBtn');
            const status = document.getElementById('manualStatus');
            btn.disabled = true;
            status.querySelector('.status-text').textContent = 'در حال آپلود...';
            status.querySelector('.icon').innerHTML = '<span class="spinner"></span>';
            
            fetch('/api/upload', { method: 'POST', body: formData })
                .then(response => response.json())
                .then(data => {
                    if (data.success) {
                        status.querySelector('.status-text').textContent = '✅ آپلود موفق! در حال به‌روزرسانی...';
                        status.querySelector('.icon').textContent = '📥';
                    } else {
                        status.querySelector('.status-text').textContent = '❌ خطا: ' + (data.error || 'Unknown error');
                        status.querySelector('.icon').textContent = '❌';
                        btn.disabled = false;
                    }
                })
                .catch(error => {
                    status.querySelector('.status-text').textContent = '❌ خطا: ' + error.message;
                    status.querySelector('.icon').textContent = '❌';
                    btn.disabled = false;
                });
        };

        document.querySelector('input[type="file"]').addEventListener('change', function(e) {
            document.getElementById('fileName').textContent = this.files[0] ? this.files[0].name : 'هیچ فایلی انتخاب نشده';
        });

        function showSuccessMessage(message) {
            const overlay = document.getElementById('successMessage');
            if (overlay) {
                overlay.classList.remove('hidden');
                overlay.querySelector('h2').textContent = message || 'به‌روزرسانی با موفقیت انجام شد!';
            }
        }

        function startRedirectTimer() {
            countdown = 5;
            const countdownText = document.getElementById('countdownText');
            const ring = document.getElementById('countdownRing');
            const circumference = 283;
            
            if (redirectTimer) clearInterval(redirectTimer);
            redirectTimer = setInterval(() => {
                countdown--;
                if (countdownText) countdownText.textContent = countdown;
                if (ring) {
                    const offset = circumference - (countdown / 5) * circumference;
                    ring.style.strokeDashoffset = offset;
                }
                if (countdown <= 0) {
                    clearInterval(redirectTimer);
                    window.location.href = '/';
                }
            }, 1000);
            
            setTimeout(() => {
                fetch('/api/restart', { method: 'POST' }).catch(() => {});
            }, 1000);
        }

        document.addEventListener('DOMContentLoaded', function() {
            connectEventSource();
            fetch('/api/version')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('currentVersion').textContent = data.current || '---';
                    document.getElementById('installedVersion').textContent = data.current || '---';
                })
                .catch(() => {
                    document.getElementById('currentVersion').textContent = 'Error';
                });
        });
    </script>
</body>
</html>
)rawliteral";

// ============================================
// کلاس WebServer
// ============================================

WebServer::WebServer() 
    : m_server(WEB_SERVER_PORT)
    , m_events("/events")
    , m_updateInProgress(false)
    , m_updateStatus("Ready")
    , m_updateProgress(0) {
}

WebServer::~WebServer() {
}

bool WebServer::begin() {
    setupRoutes();
    m_server.addHandler(&m_events);
    m_server.begin();
    DEBUG_PRINTLN("✅ Web Server started on port " + String(WEB_SERVER_PORT));
    DEBUG_PRINT("🌐 URL: http://");
    DEBUG_PRINTLN(WiFi.localIP());
    return true;
}

void WebServer::setupRoutes() {
    m_server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->handleRoot(request);
    });
    
    m_server.on("/ota", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->handleOTA(request);
    });
    
    m_server.on("/api/version", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->handleVersionCheck(request);
    });
    
    m_server.on("/api/update", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->handleStartUpdate(request);
    });
    
    m_server.on("/api/restart", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->handleRestart(request);
    });
    
    m_server.on("/api/upload", HTTP_POST, 
        [](AsyncWebServerRequest* request) {},
        [this](AsyncWebServerRequest* request, 
               const String& filename, 
               size_t index, 
               uint8_t* data, 
               size_t len, 
               bool final) {
            this->handleUpload(request, filename, index, data, len, final);
        }
    );
    
    m_events.onConnect([this](AsyncEventSourceClient* client) {
        this->handleEvents(client);
    });
    
    m_server.onNotFound([this](AsyncWebServerRequest* request) {
        this->handleNotFound(request);
    });
}

void WebServer::handleRoot(AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", INDEX_HTML);
}

void WebServer::handleOTA(AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", OTA_HTML);
}

void WebServer::handleVersionCheck(AsyncWebServerRequest* request) {
    DynamicJsonDocument doc(256);
    doc["current"] = FIRMWARE_VERSION;
    doc["latest"] = otaHandler.getLatestVersion();
    doc["updateAvailable"] = otaHandler.isUpdateAvailable();
    doc["status"] = otaHandler.getStatusMessage();
    doc["progress"] = otaHandler.getProgress();
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void WebServer::handleStartUpdate(AsyncWebServerRequest* request) {
    if (!otaHandler.isUpdateAvailable()) {
        DynamicJsonDocument doc(128);
        doc["success"] = false;
        doc["error"] = "No update available";
        String response;
        serializeJson(doc, response);
        request->send(400, "application/json", response);
        return;
    }
    
    if (m_updateInProgress) {
        DynamicJsonDocument doc(128);
        doc["success"] = false;
        doc["error"] = "Update already in progress";
        String response;
        serializeJson(doc, response);
        request->send(409, "application/json", response);
        return;
    }
    
    m_updateInProgress = true;
    bool success = otaHandler.startUpdate(
        "http://" + String(OTA_HOST) + OTA_PATH + OTA_FIRMWARE_FILE
    );
    m_updateInProgress = false;
    
    DynamicJsonDocument doc(128);
    doc["success"] = success;
    if (!success) {
        doc["error"] = otaHandler.getStatusMessage();
    }
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
    
    if (success) {
        delay(1000);
        ESP.restart();
    }
}

void WebServer::handleUpload(AsyncWebServerRequest* request, 
                            const String& filename, 
                            size_t index, 
                            uint8_t* data, 
                            size_t len, 
                            bool final) {
    if (!isAuthenticated(request)) {
        request->send(401);
        return;
    }
    
    if (m_updateInProgress) {
        request->send(409, "application/json", "{\"success\":false,\"error\":\"Update in progress\"}");
        return;
    }
    
    if (!filename.endsWith(".bin")) {
        request->send(400, "application/json", "{\"success\":false,\"error\":\"Invalid file type\"}");
        return;
    }
    
    m_updateInProgress = true;
    bool success = otaHandler.processManualUpdate(data, len, final);
    
    if (final) {
        m_updateInProgress = false;
        DynamicJsonDocument doc(128);
        doc["success"] = success;
        if (!success) {
            doc["error"] = otaHandler.getStatusMessage();
        }
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
        
        if (success) {
            if (m_onUpdateComplete) {
                m_onUpdateComplete();
            }
            delay(1000);
            ESP.restart();
        }
    } else {
        DynamicJsonDocument doc(64);
        doc["success"] = true;
        doc["progress"] = otaHandler.getProgress();
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    }
}

void WebServer::handleRestart(AsyncWebServerRequest* request) {
    request->send(200, "application/json", "{\"success\":true}");
    delay(500);
    ESP.restart();
}

void WebServer::handleNotFound(AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Not Found");
}

void WebServer::handleEvents(AsyncEventSourceClient* client) {
    DEBUG_PRINTLN("✅ EventSource client connected");
    client->send("{\"type\":\"connected\",\"message\":\"Connected to event stream\"}", "message", millis());
}

void WebServer::sendUpdateProgress(int progress, const String& status) {
    m_updateProgress = progress;
    m_updateStatus = status;
    
    DynamicJsonDocument doc(256);
    doc["type"] = (progress >= 100) ? "complete" : "progress";
    doc["progress"] = progress;
    doc["status"] = status;
    if (progress >= 100) {
        doc["message"] = "Update completed successfully!";
    }
    
    String json;
    serializeJson(doc, json);
    m_events.send(json.c_str(), "message", millis());
}

bool WebServer::isAuthenticated(AsyncWebServerRequest* request) {
    return true;
}
