extern crate log;
use zenoh::config::{Config, ConnectConfig, EndPoint, WhatAmI};
use zenoh::prelude::sync::*;
use std::str::FromStr;
use serde_json;
use std::env;

pub fn get_zenoh_config() -> Option<Config> {
    let mut config =  Config::default();

    let bus = match env::var("ZABBIX_ZBUS_ENDPOINT") {
        Ok(val) => val,
        Err(_) => "tcp/127.0.0.1:7447".to_string(),
    };
    match EndPoint::from_str(&bus) {
        Ok(zconn) => {
            log::debug!("ZENOH bus set to: {:?}", &zconn);
            let _ = config.set_connect(ConnectConfig::new(vec![zconn]).unwrap());
        }
        Err(err) => {
            log::error!("Failure in parsing connect address: {:?}", err);
            return None;
        }
    }
    let _ = config.scouting.multicast.set_enabled(Some(false));
    let _ = config.set_mode(Some(WhatAmI::Client));
    Some(config)
}

pub fn zenoh_connect(conf: Config) -> Option<Session> {
    match zenoh::open(conf).res() {
        Ok(session) => {
            log::debug!("Connection to bus was succesful");
            return Some(session);
        }
        Err(err) => {
            log::error!("Error in establishing zbus session: {:?}", err);
            return None;
        }
    }
}
