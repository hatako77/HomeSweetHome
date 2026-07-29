#ifndef ROOM_CSS_H
#define ROOM_CSS_H

#include <pgmspace.h>

const char ROOM_CSS[] PROGMEM = R"rawliteral(
.channel-card{
    background:var(--card-bg);
    border:1px solid var(--border-color);
    border-radius:5px 10px;
    padding:6px;
    display:flex;
    flex-direction:column;
    gap:14px;
    background: #111827;
    border: 1px solid #1f2937;
    margin-bottom: 15px;
}
.device-status
{
    display:flex;
    align-items:center;
    gap:6px;
}

.status-dot
{
    width:8px;
    height:8px;
    border-radius:50%;
    display:inline-block;
}

.status-dot.online
{
    margin-top: 0.6rem;
    background:#2ecc71;
    box-shadow:0 0 6px #2ecc71;
}

.status-dot.offline
{
    background:#6b7280;
}
.channel-header{
    display:flex;
    align-items:center;
    gap:8px;
}

.channel-address{
    font-size:0.8rem;
    direction: ltr;

}
.channel-info{
    color: #969696;
    font-weight: 600;
    flex:1;
    display:flex;
    flex-direction:column;
    gap:6px;
}

.channel-name{
    font-size:1.1rem;
    font-weight:600;
}

.channel-device{
    display:flex;
    align-items:center;
    gap:10px;
    color:var(--text-secondary);
    font-size:13px;
}

.device-status{
    width:10px;
    height:10px;
    border-radius:50%;
}

.device-status.online{
    background:#22c55e;
    box-shadow:0 0 8px rgba(34,197,94,.5);
}

.device-status.offline{
    background:#6b7280;
}

.channel-actions{
    display:inline;
    border-right-style: solid;
    border-color: #1f2937;
    padding-right: 6px;
    border-width: 2px;
}

.icon-btn{
    width:38px;
    height:38px;
    border:none;
    border-radius:10px;
    background:rgba(255,255,255,.05);
    display:flex;
    align-items:center;
    justify-content:center;
    cursor:pointer;
    transition:.2s;
}


.channel-footer{
    display:flex;
    justify-content:space-between;
    padding-top:12px;
    border-top:1px solid var(--border-color);
    color:var(--text-secondary);
    font-size:14px;
}
#roomsContainer{
    display:grid;
    grid-template-columns:repeat(auto-fit,minmax(360px,1fr));
    gap:20px;
    align-items:start;
}

.room-card{
    background:#111827;
    border:1px solid #1f2937;
    border-radius:18px;
    overflow:hidden;
    transition:.25s;
    box-shadow:0 8px 24px rgba(0,0,0,.25);
}

.room-title{
    display:flex;
    align-items:center;
    gap:12px;
    width: 100%;
    justify-content: space-between;
}

.room-header{
    display:flex;
    justify-content:space-between;
    align-items:center;
    padding:8px 15px;
    border-bottom:1px solid #1f2937;
    font-size: 1.3rem;
    font-weight: 800;
}


.room-name{
    display:flex;
    align-items:center;
    gap:10px;
    font-size:1rem;
    font-weight:600;
}

.room-actions{
    display:flex;
    align-items:center;
    gap:8px;
}

.room-count{
    padding:4px 10px;
    border-radius:999px;
    background:#1e293b;
    color:#94a3b8;
    font-size:1rem;
}

.channel-list{
    display:grid;
    grid-template-columns:repeat(auto-fill,minmax(130px,1fr));
    gap:12px;
    padding:14px;
}

.channel-tile{
    width:100%;
}

.tile{
    display:flex;
    flex-direction:column;
    align-items:center;
    justify-content:center;
    gap:10px;

    height:130px;

    padding:16px;

    border-radius:16px;

    background:#0b1220;

    border:1px solid #1f2937;

    transition:.25s;

    cursor:pointer;

    user-select:none;
}

.tile.on{
    background:#155e33;
    border-color:#22c55e;
}

.tile.off{
    background:#111827;
}

.tile.disabled{
    opacity:.35;
}

.channel-icon{
    width:52px;
    height:52px;

    display:flex;
    align-items:center;
    justify-content:center;

    border-radius:14px;

    background:#1e293b;

    transition:.25s;
}

.tile.on .channel-icon{
    background:#14532d;
}

.channel-icon svg{
    width:26px;
    height:26px;
    fill:#94a3b8;
    transition:.25s;
}

.tile.on .channel-icon svg{
    fill:#22c55e;
}

.channel-text{
    display:flex;
    flex-direction:column;
    align-items:center;
    gap:6px;
}

.channel-state{
    font-size:1rem;
    color:#94a3b8;
    transition:.25s;
}

.channel-state.on{
    color:#22c55e;
    font-weight:bold;
}

.channel-state.off{
    color:#ef4444;
    font-weight:bold;
}

.icon-btn{
    width:32px;
    height:32px;
    display:flex;
    align-items:center;
    justify-content:center;
    border:none;
    border-radius:8px;
    cursor:pointer;
}

.icon-btn.danger{
    color:#e74c3c;
}

@media(max-width:768px){

    #roomsContainer{
        grid-template-columns:1fr;
    }

    .channel-list{
        grid-template-columns:repeat(2,1fr);
    }

}

)rawliteral";

#endif
