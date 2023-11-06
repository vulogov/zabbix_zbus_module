extern crate log;
use libc::c_int;
use env_logger::{Env};

use crate::zbus_zenoh;

#[no_mangle]
pub extern fn zbus_module_init() -> c_int {
    let env = Env::default()
        .filter_or("ZBUS_LOG_LEVEL", "debug");
    env_logger::init_from_env(env);
    match zbus_zenoh::get_zenoh_config() {
        Some(config) => {
            log::debug!("zbus config has been generated");
            match zbus_zenoh::zenoh_connect(config.clone()) {
                Some(session) => {
                    log::debug!("zbus is ready");
                }
                None => {
                    log::error!("Unable to connect to zbus");
                    return 0;
                }
            }
        }
        None => {
            log::error!("Unable to obtain zbus config");
            return 0;
        }
    }
    log::debug!("Initialize ZBUS module");
    return 1;
}
