use libc::c_int;
use std::time::{SystemTime};

use crate::zbus_zenoh;

#[no_mangle]
pub extern fn zbus_module_ping() -> c_int {
    let s_duration_since_epoch = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap();
    let start_timestamp = s_duration_since_epoch.as_nanos();
    match zbus_zenoh::get_zenoh_config() {
        Some(config) => {
            match zbus_zenoh::zenoh_connect(config.clone()) {
                Some(session) => {
                    let _ = session.close();
                    let e_duration_since_epoch = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap();
                    let end_timestamp = e_duration_since_epoch.as_nanos();
                    return (end_timestamp-start_timestamp).try_into().unwrap();
                }
                None => {
                    return 0;
                }
            }
        }
        None => {
            return 0;
        }
    }
}
