// ===== Global State =====
const state = {
    startTime: Date.now(),
    version: '---',
    ip: '---',
    signal: '---'
};

// ===== Initialize =====
document.addEventListener('DOMContentLoaded', () => {
    fetchSystemInfo();
    startUptimeTimer();
    startSystemInfoTimer();
});

// ===== System Info =====
function fetchSystemInfo() {
    fetch('/api/version')
        .then(response => response.json())
        .then(data => {
            state.version = data.current || '---';
            document.getElementById('version').textContent = state.version;
            document.getElementById('footerVersion').textContent = state.version;
        })
        .catch(error => {
            console.error('Error fetching version:', error);
            document.getElementById('version').textContent = 'Error';
        });
}

// ===== Uptime =====
function startUptimeTimer() {
    updateUptime();
    setInterval(updateUptime, 1000);
}

function updateUptime() {
    const elapsed = Math.floor((Date.now() - state.startTime) / 1000);
    const days = Math.floor(elapsed / 86400);
    const hours = Math.floor((elapsed % 86400) / 3600);
    const minutes = Math.floor((elapsed % 3600) / 60);
    const seconds = elapsed % 60;
    
    let uptimeStr = '';
    if (days > 0) uptimeStr += `${days}d `;
    uptimeStr += `${String(hours).padStart(2, '0')}:${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`;
    
    document.getElementById('uptime').textContent = uptimeStr;
}

// ===== System Info Timer =====
function startSystemInfoTimer() {
    fetchSystemInfoDetails();
    setInterval(fetchSystemInfoDetails, 10000);
}

function fetchSystemInfoDetails() {
    // IP Address (from browser)
    fetch('/api/ip')
        .then(response => response.json())
        .then(data => {
            state.ip = data.ip || '---';
            document.getElementById('ip').textContent = state.ip;
        })
        .catch(() => {
            // Fallback: get IP from location
            const url = window.location.href;
            const ipMatch = url.match(/\/\/([\d.]+)/);
            if (ipMatch) {
                document.getElementById('ip').textContent = ipMatch[1];
            }
        });
    
    // Signal strength simulation
    const signalStrength = Math.floor(60 + Math.random() * 40);
    document.getElementById('signal').textContent = `${signalStrength}%`;
    
    // Temperature simulation
    const temp = (35 + Math.random() * 15).toFixed(1);
    document.getElementById('temperature').textContent = `${temp}°C`;
    
    // Memory simulation
    const memory = (100 + Math.random() * 200).toFixed(0);
    document.getElementById('memory').textContent = `${memory} KB`;
}

// ===== Actions =====
function restartDevice() {
    if (!confirm('آیا از راه‌اندازی مجدد دستگاه اطمینان دارید؟')) return;
    
    const btn = document.querySelector('.btn-warning');
    btn.disabled = true;
    btn.innerHTML = '<span class="icon">⏳</span> در حال راه‌اندازی...';
    
    fetch('/api/restart', { method: 'POST' })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                showToast('✅ دستگاه در حال راه‌اندازی مجدد...');
                setTimeout(() => {
                    window.location.reload();
                }, 5000);
            } else {
                showToast('❌ خطا در راه‌اندازی مجدد');
                btn.disabled = false;
                btn.innerHTML = '<span class="icon">🔁</span> راه‌اندازی مجدد';
            }
        })
        .catch(error => {
            console.error('Error restarting:', error);
            showToast('❌ خطا در ارتباط با دستگاه');
            btn.disabled = false;
            btn.innerHTML = '<span class="icon">🔁</span> راه‌اندازی مجدد';
        });
}

function refreshData() {
    const btn = document.querySelector('.btn-info');
    btn.disabled = true;
    btn.innerHTML = '<span class="icon">⏳</span> در حال به‌روزرسانی...';
    
    fetchSystemInfo();
    fetchSystemInfoDetails();
    
    setTimeout(() => {
        btn.disabled = false;
        btn.innerHTML = '<span class="icon">🔄</span> به‌روزرسانی';
        showToast('✅ اطلاعات به‌روز شد');
    }, 1000);
}

// ===== Toast Notification =====
function showToast(message) {
    const existing = document.querySelector('.toast');
    if (existing) existing.remove();
    
    const toast = document.createElement('div');
    toast.className = 'toast';
    toast.textContent = message;
    Object.assign(toast.style, {
        position: 'fixed',
        bottom: '20px',
        left: '50%',
        transform: 'translateX(-50%)',
        background: 'rgba(0,0,0,0.8)',
        color: 'white',
        padding: '12px 24px',
        borderRadius: '50px',
        fontSize: '0.95rem',
        zIndex: '999',
        animation: 'slideUp 0.3s ease',
        backdropFilter: 'blur(10px)',
        boxShadow: '0 4px 20px rgba(0,0,0,0.2)'
    });
    
    document.body.appendChild(toast);
    
    setTimeout(() => {
        toast.style.opacity = '0';
        toast.style.transition = 'opacity 0.3s ease';
        setTimeout(() => toast.remove(), 300);
    }, 3000);
}
