use evdev::EventType;
use evdev::{Device, Key};
use std::time::Duration;

pub fn list_devices() {
    let enumerator = evdev::enumerate();
    for d in enumerator {
        match Device::open(&d.0) {
            Ok(device) => {
                println!(
                    "Device {:?} - `{}`",
                    &d.0,
                    device.name().unwrap_or("Unknown device")
                );
            }
            Err(e) => {
                println!("Unable to open device {:?} - {}", &d.0, e);
            }
        };
    }
}

pub fn find_keyboard() -> Option<Device> {
    list_devices();
    let mut enumerator = evdev::enumerate();
    loop {
        let device_enum = enumerator.next();
        match device_enum {
            Some(denum) => {
                print!("Found device {:?} - ", &denum.0);
                let device = match Device::open(&denum.0) {
                    Ok(d) => d,
                    Err(e) => {
                        println!("Unable to open device {:?} - {}", &denum.0, e);
                        continue;
                    }
                };
                if !device.name().unwrap_or("").contains("virtual")
                    && device.supported_keys().map_or(false, |key| {
                        key.contains(Key::KEY_ENTER) && key.contains(Key::KEY_Q)
                    })
                {
                    println!("`{}` - OK", device.name().unwrap_or("Unknown device"));
                    break Some(device);
                } else {
                    println!("`{}` - NO", device.name().unwrap_or("Unknown device"));
                }
            }
            None => break None,
        }
    }
}

fn main() {
    let mut port = match serialport::new("/dev/ttyUSB0", 115_200)
        .timeout(Duration::from_millis(10))
        .open()
    {
        Ok(port) => port,
        Err(e) => {
            eprintln!("Failed to open \"{}\". Error: {}", "/dev/ttyUSB0", e);
            return;
        }
    };

    let output = "Hello World!\n".to_string().into_bytes();
    port.write(&output).expect("Write failed!");
    return;

    match find_keyboard() {
        Some(mut keyboard) => loop {
            match keyboard.fetch_events() {
                Ok(events) => {
                    for event in events {
                        if event.value() == 1 {
                            if event.code() == 17 {
                                let output = 'w'.to_string().into_bytes();
                                port.write(&output).expect("Write failed!");
                            } else if event.code() == 31 {
                                let output = 's'.to_string().into_bytes();
                                port.write(&output).expect("Write failed!");
                            } else if event.code() == 23 {
                                let output = 'i'.to_string().into_bytes();
                                port.write(&output).expect("Write failed!");
                            } else if event.code() == 37 {
                                let output = 'k'.to_string().into_bytes();
                                port.write(&output).expect("Write failed!");
                            } else if event.code() == 19 {
                                let output = 'r'.to_string().into_bytes();
                                port.write(&output).expect("Write failed!");
                            } else if event.code() == 39 {
                                // Change mode (random / manual)
                                let output = 'm'.to_string().into_bytes();
                                port.write(&output).expect("Write failed!");
                            }
                        }
                    }
                }
                Err(_) => {
                    println!("Error fetching events");
                }
            }
        },
        None => {
            println!("No keyboard could be found");
        }
    }
}
