extern crate log;
use libc::{c_int, c_char};
use std::ffi::{CString, CStr};

use crate::zbus_zenoh;

#[no_mangle]
pub unsafe extern fn free_string(ptr: *const c_char) {
    let _ = CString::from_raw(ptr as *mut _);
}

#[no_mangle]
pub extern fn zbus_module_get_str(key: *const c_char) -> *const c_char {
    match unsafe { CStr::from_ptr(key).to_str() } {
        Ok(zbus_key) => {
            match zbus_zenoh::get(zbus_key.to_string()) {
                Some(res) => {
                    println!("{:?}", &res);
                    match res.get("value") {
                        Some(value) => {
                            return CString::new(value.to_string()).expect("BOO").into_raw();
                        }
                        None => {
                            return std::ptr::null();
                        }
                    }
                }
                None => {
                    return std::ptr::null();
                }
            }
        }
        Err(err) => {
            log::error!("Error converting key: {:?}", err);
            return std::ptr::null();
        }
    }
}
