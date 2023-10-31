all: debug

debug: zbus.c
	cargo build
	gcc -fPIC -shared -o zbus.so zbus.c -I. -I../zabbix/include -I../zabbix/include/common -L./target/debug -lzbus

clean:
	rm -f zbus.so
