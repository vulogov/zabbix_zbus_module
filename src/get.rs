extern crate log;
use libc::{c_char};
use std::ffi::{CString, CStr};

use crate::zbus_zenoh;

fn get_value_v2(data: &serde_json::Value) -> Option<serde_json::Value> {
    match data.get("body") {
        Some(body) => {
            match body.get("details") {
                Some(details1) => {
                    match details1.get("details") {
                        Some(details) => {
                            match details.get("data") {
                                Some(data) => { return Some(data.clone()); }
                                None => { return None; }
                            }
                        }
                        None => { return None; }
                    }
                }
                None => { return None; }
            }
        }
        None => { return None; }
    }
}

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
                    match get_value_v2(&res) {
                        Some(value) => {
                            return CString::new(value.to_string()).expect("Expecting a value").into_raw();
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

#[no_mangle]
pub extern fn zbus_module_get_raw(key: *const c_char) -> *const c_char {
    match unsafe { CStr::from_ptr(key).to_str() } {
        Ok(zbus_key) => {
            match zbus_zenoh::get(zbus_key.to_string()) {
                Some(res) => {
                    return CString::new(res.to_string()).expect("Expecting a value").into_raw();
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
