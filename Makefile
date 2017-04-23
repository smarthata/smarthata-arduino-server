run-docker:
	docker run --rm -v /home/valery/Dropbox/robotics/arduino/projects/smarthata/web:/usr/share/nginx/html -p 80:80 nginx

html2h:
	./html2h.py web > src/web.h

html2h-nostrip:
	./html2h.py web --nostrip > src/web.h

pio-run: html2h
	pio run
