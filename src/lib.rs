use libc::c_int;

#[no_mangle]
pub extern fn zrhai_module_init() {
    println!("Rust here");
}

#[no_mangle]
pub extern fn zrhai_module_ping() -> c_int {
    1
}
