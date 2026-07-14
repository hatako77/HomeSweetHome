#ifndef FORM_CSS_H
#define FORM_CSS_H

#include <pgmspace.h>

const char FORM_CSS[] PROGMEM=R"rawliteral(

.form-group{
	display:flex;
	flex-direction:column;
	gap:8px;
	margin-bottom:18px;
}

.form-row{
	display:flex;
	gap:16px;
}

.form-row>*{
	flex:1;
}

label{
	font-size:13px;
	font-weight:600;
	color:#94a3b8;
}

.input,
select,
textarea{
	width:100%;
	padding:12px 14px;
	background:#111827;
	border:1px solid #334155;
	border-radius:12px;
	color:#f8fafc;
	font-size:14px;
	transition:.2s;
}

.input:focus,
select:focus,
textarea:focus{
	border-color:#38bdf8;
	box-shadow:0 0 0 3px rgba(56,189,248,.15);
}

.input::placeholder,
textarea::placeholder{
	color:#64748b;
}

textarea{
	min-height:120px;
	resize:vertical;
}

input[type="checkbox"]{
	width:18px;
	height:18px;
	cursor:pointer;
}

.checkbox{
	display:flex;
	align-items:center;
	gap:10px;
	margin:10px 0;
}

.checkbox label{
	margin:0;
	cursor:pointer;
}

.form-actions{
	display:flex;
	justify-content:flex-end;
	gap:12px;
	margin-top:24px;
}

@media(max-width:768px){

	.form-row{
		flex-direction:column;
		gap:0;
	}

	.form-actions{
		flex-direction:column;
	}

	.form-actions .btn{
		width:100%;
	}

}

)rawliteral";

#endif
