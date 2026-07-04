// ===== Global State =====
const state = {
    eventSource: null,
    redirectTimer: null,
    countdown: 5,
    updateInProgress: false,
    currentVersion: '---'
};

// ===== Initialize =====
document.addEventListener('DOMContentLoaded', () => {
    connectEventSource();
    fetchCurrentVersion();
    setupFileInput();
    setupUploadForm();
});

// ===== EventSource Connection =====
function connectEventSource() {
    if (state.eventSource) {
        state.eventSource.close();
    }
    
    state.eventSource = new EventSource('/events');
    
    state.eventSource.onopen = () => {
        console.log('✅ EventSource connected');
    };
    
    state.eventSource.onmessage = (e) => {
        try {
            const data = JSON.parse(e.data);
            handleEvent(data);
        } catch (err) {
            console.error('Error parsing event:', err);
        }
    };
    
    state.eventSource.onerror = (e) => {
        console.error('EventSource error:', e);
        if (e.target.readyState === EventSource.CLOSED) {
            console.log('EventSource closed, reconnecting...');
            setTimeout(connectEventSource, 5000);
        }
    };
}

// ===== Handle Events =====
function handleEvent(data) {
    const { type, progress, status, message } = data;
    
    if (type === 'progress') {
        updateProgress(progress, status);
    } else if (type === 'complete') {
        updateProgress(100, '✅ ' + (message || 'Update complete!'));
        showSuccessMessage(message || 'Update completed successfully!');
        startRedirectTimer();
    } else if (type === 'connected') {
        console.log('Connected to event stream');
    }
}

// ===== Update Progress =====
function updateProgress(progress, status) {
    // Auto section
    const autoProgress = document.getElementById('autoProgress');
    const autoStatus = document.getElementById('autoStatus');
    
    if (autoProgress) {
        const progressText = autoProgress.querySelector('.progress-text');
        autoProgress.style.width = Math.min(progress, 100) + '%';
        if (progressText) progressText.textContent = Math.min(progress, 100) + '%';
    }
    
    if (autoStatus) {
        autoStatus.querySelector('.status-text').textContent = status;
        if (progress > 0 && progress < 100) {
            autoStatus.querySelector('.icon').innerHTML = '<span class="spinner"></span>';
        } else {
            autoStatus.querySelector('.icon').textContent = progress >= 100 ? '✅' : '⏳';
        }
    }
    
    // Manual section
    const manualProgress = document.getElementById('manualProgress');
    const manualStatus = document.getElementById('manualStatus');
    
    if (manualProgress) {
        const progressText = manualProgress.querySelector('.progress-text');
        manualProgress.style.width = Math.min(progress, 100) + '%';
        if (progressText) progressText.textContent = Math.min(progress, 100) + '%';
    }
    
    if (manualStatus) {
        manualStatus.querySelector('.status-text').textContent = status;
        if (progress > 0 && progress < 100) {
            manualStatus.querySelector('.icon').innerHTML = '<span class="spinner"></span>';
        } else {
            manualStatus.querySelector('.icon').textContent = progress >= 100 ? '✅' : '📤';
        }
    }
    
    // Disable buttons during update
    if (progress > 0 && progress < 100) {
        state.updateInProgress = true;
        document.querySelectorAll('.btn').forEach(btn => btn.disabled = true);
    } else if (progress >= 100) {
        state.updateInProgress = false;
        document.querySelectorAll('.btn').forEach(btn => btn.disabled = true);
    } else {
        state.updateInProgress = false;
        document.querySelectorAll('.btn').forEach(btn => btn.disabled = false);
    }
}

// ===== Check for Updates =====
function checkUpdate() {
    if (state.updateInProgress) {
        showToast('⏳ در حال به‌روزرسانی...');
        return;
    }
    
    const btn = document.getElementById('checkBtn');
    const status = document.getElementById('autoStatus');
    
    btn.disabled = true;
    status.querySelector('.status-text').textContent = 'در حال بررسی نسخه جدید...';
    status.querySelector('.icon').innerHTML = '<span class="spinner"></span>';
    
    fetch('/api/version')
        .then(response => response.json())
        .then(data => {
            const latestVersion = document.getElementById('latestVersion');
            const updateBtn = document.getElementById('updateBtn');
            const installedVersion = document.getElementById('installedVersion');
            
            if (latestVersion) {
                latestVersion.textContent = data.latest || '---';
            }
            if (installedVersion) {
                installedVersion.textContent = data.current || '---';
            }
            
            if (data.updateAvailable) {
                status.querySelector('.status-text').textContent = '✅ نسخه جدید موجود است!';
                status.querySelector('.icon').textContent = '🎉';
                updateBtn.classList.remove('hidden');
                updateBtn.disabled = false;
                document.getElementById('deviceStatus').textContent = '🔄 به‌روزرسانی موجود';
            } else {
                status.querySelector('.status-text').textContent = '✅ دستگاه به‌روز است';
                status.querySelector('.icon').textContent = '✅';
                updateBtn.classList.add('hidden');
                document.getElementById('deviceStatus').textContent = '🟢 فعال';
            }
            
            btn.disabled = false;
        })
        .catch(error => {
            console.error('Error checking update:', error);
            status.querySelector('.status-text').textContent = '❌ خطا: ' + error.message;
            status.querySelector('.icon').textContent = '❌';
            btn.disabled = false;
        });
}

// ===== Start Automatic Update =====
function startUpdate() {
    if (state.updateInProgress) {
        showToast('⏳ در حال به‌روزرسانی...');
        return;
    }
    
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
                showToast('✅ به‌روزرسانی در حال انجام...');
            } else {
                status.querySelector('.status-text').textContent = '❌ خطا: ' + (data.error || 'Unknown error');
                status.querySelector('.icon').textContent = '❌';
                btn.disabled = false;
                showToast('❌ خطا در شروع به‌روزرسانی');
            }
        })
        .catch(error => {
            console.error('Error starting update:', error);
            status.querySelector('.status-text').textContent = '❌ خطا: ' + error.message;
            status.querySelector('.icon').textContent = '❌';
            btn.disabled = false;
            showToast('❌ خطا در ارتباط با دستگاه');
        });
}

// ===== Manual Upload =====
function setupFileInput() {
    const fileInput = document.querySelector('input[type="file"]');
    const fileName = document.getElementById('fileName');
    
    if (fileInput && fileName) {
        fileInput.addEventListener('change', (e) => {
            const file = e.target.files[0];
            fileName.textContent = file ? file.name : 'هیچ فایلی انتخاب نشده';
            
            if (file && !file.name.endsWith('.bin')) {
                showToast('⚠️ لطفاً فایل با پسوند .bin انتخاب کنید');
                fileInput.value = '';
                fileName.textContent = 'هیچ فایلی انتخاب نشده';
            }
        });
    }
}

function setupUploadForm() {
    const form = document.getElementById('uploadForm');
    if (!form) return;
    
    form.addEventListener('submit', (e) => {
        e.preventDefault();
        
        if (state.updateInProgress) {
            showToast('⏳ در حال به‌روزرسانی...');
            return;
        }
        
        const fileInput = form.querySelector('input[type="file"]');
        const file = fileInput.files[0];
        
        if (!file) {
            showToast('⚠️ لطفاً یک فایل انتخاب کنید');
            return;
        }
        
        if (!file.name.endsWith('.bin')) {
            showToast('⚠️ لطفاً فایل با پسوند .bin انتخاب کنید');
            return;
        }
        
        const formData = new FormData();
        formData.append('firmware', file);
        
        const btn = document.getElementById('manualUploadBtn');
        const status = document.getElementById('manualStatus');
        
        btn.disabled = true;
        status.querySelector('.status-text').textContent = 'در حال آپلود...';
        status.querySelector('.icon').innerHTML = '<span class="spinner"></span>';
        
        fetch('/api/upload', {
            method: 'POST',
            body: formData
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                status.querySelector('.status-text').textContent = '✅ آپلود موفق! در حال به‌روزرسانی...';
                status.querySelector('.icon').textContent = '📥';
                showToast('✅ فایل آپلود شد، در حال به‌روزرسانی...');
            } else {
                status.querySelector('.status-text').textContent = '❌ خطا: ' + (data.error || 'Unknown error');
                status.querySelector('.icon').textContent = '❌';
                btn.disabled = false;
                showToast('❌ خطا در آپلود فایل');
            }
        })
        .catch(error => {
            console.error('Error uploading:', error);
            status.querySelector('.status-text').textContent = '❌ خطا: ' + error.message;
            status.querySelector('.icon').textContent = '❌';
            btn.disabled = false;
            showToast('❌ خطا در ارتباط با دستگاه');
        });
    });
}

// ===== Fetch Current Version =====
function fetchCurrentVersion() {
    fetch('/api/version')
        .then(response => response.json())
        .then(data => {
            state.currentVersion = data.current || '---';
            document.getElementById('currentVersion').textContent = state.currentVersion;
            document.getElementById('installedVersion').textContent = state.currentVersion;
        })
        .catch(error => {
            console.error('Error fetching version:', error);
            document.getElementById('currentVersion').textContent = 'Error';
        });
}

// ===== Success Message =====
function showSuccessMessage(message) {
    const overlay = document.getElementById('successMessage');
    if (overlay) {
        overlay.classList.remove('hidden');
        overlay.querySelector('h2').textContent = message || 'به‌روزرسانی با موفقیت انجام شد!';
    }
}

// ===== Redirect Timer =====
function startRedirectTimer() {
    state.countdown = 5;
    const countdownText = document.getElementById('countdownText');
    const ring = document.getElementById('countdownRing');
    const circumference = 283;
    
    if (state.redirectTimer) {
        clearInterval(state.redirectTimer);
    }
    
    state.redirectTimer = setInterval(() => {
        state.countdown--;
        
        if (countdownText) {
            countdownText.textContent = state.countdown;
        }
        
        if (ring) {
            const offset = circumference - (state.countdown / 5) * circumference;
            ring.style.strokeDashoffset = offset;
        }
        
        if (state.countdown <= 0) {
            clearInterval(state.redirectTimer);
            window.location.href = '/';
        }
    }, 1000);
    
    // Restart ESP after 1 second
    setTimeout(() => {
        fetch('/api/restart', { method: 'POST' }).catch(() => {});
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
        boxShadow: '0 4px 20px rgba(0,0,0,0.2)',
        maxWidth: '90%',
        textAlign: 'center'
    });
    
    document.body.appendChild(toast);
    
    setTimeout(() => {
        toast.style.opacity = '0';
        toast.style.transition = 'opacity 0.3s ease';
        setTimeout(() => toast.remove(), 300);
    }, 3000);
}
