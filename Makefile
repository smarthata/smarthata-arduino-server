WEB_INPUT=web
WEB_OUTPUT=web-out
SRC_WEB_H=src/web.h

run-docker:
	docker run --rm -v /home/valery/Dropbox/robotics/arduino/projects/smarthata/web:/usr/share/nginx/html -p 80:80 nginx

web-install:
	sudo apt install npm
	npm install html-minifier -g

web-minimize:
	html-minifier --input-dir ${WEB_INPUT} --output-dir ${WEB_OUTPUT} --collapse-whitespace
	cp ${WEB_INPUT}/favicon.ico ${WEB_OUTPUT}/favicon.ico

html2h: web-minimize
	./html2h.py ${WEB_OUTPUT} > ${SRC_WEB_H}

html2h-non-minimized:
	./html2h.py ${WEB_INPUT} > ${SRC_WEB_H}

html2h-nostrip:
	./html2h.py ${WEB_INPUT} --nostrip > ${SRC_WEB_H}

pio-run: html2h
	pio run
