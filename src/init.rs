extern crate log;
use libc::c_int;
use env_logger::{Env};


#[no_mangle]
pub extern fn zbus_module_init() -> c_int {
    let env = Env::default()
        .filter_or("ZBUS_LOG_LEVEL", "debug");
    env_logger::init_from_env(env);
    log::debug!("Initialize ZBUS module");
    return 1;
}
