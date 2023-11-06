use libc::c_int;

#[no_mangle]
pub extern fn zrhai_module_ping() -> c_int {
    42
}
