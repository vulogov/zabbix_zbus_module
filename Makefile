all: debug

debug: zbus.c
	cargo build
	gcc -fPIC -shared -o zbus_module.so zbus.c -I. -I../zabbix/include -I../zabbix/include/common -L./target/debug -lzbus_module
	find ./target/debug -maxdepth 1 -name "libzbus*.so" -o -name "libzbus*.dylib" -exec cp {} . \;

clean:
	rm -f zbus_module.so libzbus_module.*
