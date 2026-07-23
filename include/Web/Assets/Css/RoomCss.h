#ifndef ROOM_CSS_H
#define ROOM_CSS_H

#include <pgmspace.h>

const char ROOM_CSS[] PROGMEM = R"rawliteral(

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

.room-card:hover{
    transform:translateY(-4px);
    border-color:#38bdf8;
}

.room-header{
    display:flex;
    justify-content:space-between;
    align-items:center;
    padding:16px;
    background:#0f172a;
    border-bottom:1px solid #1f2937;
}

.room-name{
    display:flex;
    align-items:center;
    gap:10px;
    font-size:17px;
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
    font-size:12px;
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

.tile:hover{
    transform:translateY(-4px);
    border-color:#38bdf8;
}

.tile.on{
    background:linear-gradient(180deg,#14532d,#0b1220);
    border-color:#22c55e;
}

.tile.off{
    background:#0b1220;
}

.tile.disabled{
    opacity:.35;
    filter:grayscale(100%);
    cursor:not-allowed;
    pointer-events:none;
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

.channel-name{
    font-size:14px;
    font-weight:600;
    text-align:center;
}

.channel-state{
    font-size:12px;
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
