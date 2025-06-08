use std::env;
use std::fs;
use plotters::prelude::*;

//// Live plot tools
//use piston_window::{EventLoop, PistonWindow, WindowSettings};
//use systemstat::platform::common::Platform;
//use systemstat::System;
//use std::collections::vec_deque::VecDeque;
//mod piston_window_for_plotters;
//use piston_window_for_plotters::{draw_piston_window};

// Minifb tools
use minifb::{Key, KeyRepeat, Window, WindowOptions};
use plotters_bitmap::bitmap_pixel::BGRXPixel;
use plotters_bitmap::BitMapBackend;
use std::collections::VecDeque;
use std::error::Error;
use std::time::SystemTime;
use std::borrow::{Borrow, BorrowMut};

// https://plotters-rs.github.io/book/basic/basic_data_plotting.html
fn basic_plotting_demo() {
//
//    // Not included: Folder must already exist
//    fs::create_dir_all("images").unwrap();
//
//    // I guess these images come with the library
//    let root_area = BitMapBackend::new("images/0.1.png", (1024, 768)).into_drawing_area();
//    root_area.fill(&WHITE).unwrap();
//
//    let mut chart = ChartBuilder::on(&root_area)
//        .build_cartesian_2d(-3.14..3.14, -1.2..1.2)
//        .unwrap();
//
//    chart.draw_series(LineSeries::new(
//        (-314..314).map(|x| x as f64 / 100.0).map(|x| (x, x.sin())), &RED
//    )).unwrap();
//
//    println!("Check out \"images/0.1.png\"");
}

// https://github.com/plotters-rs/plotters-piston/blob/master/examples/cpustat.rs
fn cpu_plot() {
    //// This example uses Piston Window to render a live view of CPU Usages
    //const FPS: u32 = 10;
    //const LENGTH: u32 = 20;
    //const N_DATA_POINTS: usize = (FPS * LENGTH) as usize;
    //
    //let mut window: PistonWindow = WindowSettings::new("Real Time CPU Usage", [450, 300])
    //.samples(4)
    //.build()
    //.unwrap();
    //
    //let my_sys = System::new();
    //window.set_max_fps(FPS as u64);
    //let mut load_measurement: Vec<_> = (0..FPS).map(|_| my_sys.cpu_load().unwrap()).collect();
    //let mut epoch = 0;
    //let mut data = vec![];
    //while let Some(_) = draw_piston_window(&mut window, |b| {
    //    let cpu_loads = load_measurement[epoch % FPS as usize].done()?;
    //    let root = b.into_drawing_area();
    //    root.fill(&WHITE)?;
    //
    //    if data.len() < cpu_loads.len() {
    //        for _ in data.len()..cpu_loads.len() {
    //            data.push(VecDeque::from(vec![0f32; N_DATA_POINTS + 1]));
    //        }
    //    }
    //    for (core_load, target) in cpu_loads.into_iter().zip(data.iter_mut()) {
    //        if target.len() == N_DATA_POINTS + 1 {
    //            target.pop_front();
    //        }
    //        target.push_back(1.0 - core_load.idle);
    //    }
    //    let mut cc = ChartBuilder::on(&root)
    //        .margin(10)
    //        .caption("CPU Usage", ("sans-serif", 30))
    //        .x_label_area_size(40)
    //        .y_label_area_size(50)
    //        .build_cartesian_2d(0..N_DATA_POINTS as u32, 0f32..1f32)?;
    //    cc.configure_mesh()
    //        .x_label_formatter(&|x| format!("{}", -(LENGTH as f32) + (*x as f32 / FPS as f32)))
    //        .y_label_formatter(&|y| format!("{}%", (*y * 100.0) as u32))
    //        .x_labels(15)
    //        .y_labels(5)
    //        .x_desc("Seconds")
    //        .y_desc("% Busy")
    //        .axis_desc_style(("sans-serif", 15))
    //        .draw()?;
    //
    //    for (idx, data) in (0..).zip(data.iter()) {
    //        cc.draw_series(LineSeries::new(
    //            (0..).zip(data.iter()).map(|(a, b)| (a, *b)),
    //            &Palette99::pick(idx),
    //        ))?
    //        .label(format!("CPU {}", idx))
    //        .legend(move |(x, y)| {
    //            Rectangle::new([(x - 5, y - 5), (x + 5, y + 5)], &Palette99::pick(idx))
    //        });
    //    }
    //    cc.configure_series_labels()
    //        .background_style(&WHITE.mix(0.8))
    //        .border_style(&BLACK)
    //        .draw()?;
    //    load_measurement[epoch % FPS as usize] = my_sys.cpu_load()?;
    //    epoch += 1;
    //    Ok(())
    //}) {}
}

const W: usize = 800;
const H: usize = 600;

const SAMPLE_RATE: f64 = 10_000.0;
const FRAME_RATE: f64 = 30.0;

struct BufferWrapper(Vec<u32>);
impl Borrow<[u8]> for BufferWrapper {
    fn borrow(&self) -> &[u8] {
        unsafe {
            std::slice::from_raw_parts(
                self.0.as_ptr() as *const u8,
                self.0.len() * 4
            )
        }
    }
}
impl BorrowMut<[u8]> for BufferWrapper {
    fn borrow_mut(&mut self) -> &mut [u8] {
        unsafe {
            std::slice::from_raw_parts_mut(
                self.0.as_mut_ptr() as *mut u8,
                self.0.len() * 4
            )
        }
    }
}
impl Borrow<[u32]> for BufferWrapper {
    fn borrow(&self) -> &[u32] {
        self.0.as_slice()
    }
}
impl BorrowMut<[u32]> for BufferWrapper {
    fn borrow_mut(&mut self) -> &mut [u32] {
        self.0.as_mut_slice()
    }
}
fn get_window_title(fx: f64, fy: f64, iphase: f64) -> String {
    format!(
        "x={:.1}Hz, y={:.1}Hz, phase={:.1} +/-=Adjust y 9/0=Adjust x <Esc>=Exit",
        fx, fy, iphase
    )
}

// https://github.com/plotters-rs/plotters-minifb-demo
fn minifb_plot() {
    // This example uses my (at the moment) favorite simple window toolset
    let mut buf = BufferWrapper(vec![0u32; W * H]);
    let mut fx: f64 = 1.0;
    let mut fy: f64 = 1.1;
    let mut xphase: f64 = 0.0;
    let mut yphase: f64 = 0.1;
    let mut window = Window::new(
        &get_window_title(fx, fy, yphase - xphase),
        W,
        H,
        WindowOptions::default()
    ).unwrap();
    let cs = {
        let root = BitMapBackend::<BGRXPixel>::with_buffer_and_format(buf.borrow_mut(), (W as u32, H as u32))
            .unwrap()
            .into_drawing_area();
        root.fill(&BLACK).unwrap();
        let mut chart = ChartBuilder::on(&root)
            .margin(10)
            .set_all_label_area_size(30)
            .build_cartesian_2d(-1.2..1.2, -1.2..1.2)
            .unwrap();
        chart
            .configure_mesh()
            .label_style(("sans-serif", 15).into_font().color(&GREEN))
            .axis_style(&GREEN)
            .draw()
            .unwrap();
        let cs = chart.into_chart_state();
        root.present().unwrap();
        cs
    };
    let mut data = VecDeque::new();
    let start_ts = SystemTime::now();
    let mut last_flushed = 0.0;
    while window.is_open() && !window.is_key_down(Key::Escape) {
        let epoch = SystemTime::now()
            .duration_since(start_ts)
            .unwrap()
            .as_secs_f64();
        if let Some((ts, _, _)) = data.back() {
            if epoch - ts < 1.0 / SAMPLE_RATE {
                std::thread::sleep(std::time::Duration::from_secs_f64(epoch - ts));
                continue;
            }
            let mut ts = *ts;
            while ts < epoch {
                ts += 1.0 / SAMPLE_RATE;
                let phase_x: f64 = 2.0 * ts * std::f64::consts::PI * fx + xphase;
                let phase_y: f64 = 2.0 * ts * std::f64::consts::PI * fy + yphase;
                data.push_back((ts, phase_x.sin(), phase_y.sin()));
            }
        }
        let phase_x = 2.0 * epoch * std::f64::consts::PI * fx + xphase;
        let phase_y = 2.0 * epoch * std::f64::consts::PI * fy + yphase;
        data.push_back((epoch, phase_x.sin(), phase_y.sin()));
        if epoch - last_flushed > 1.0 / FRAME_RATE {
            {
                let root = BitMapBackend::<BGRXPixel>::with_buffer_and_format(
                    buf.borrow_mut(),
                    (W as u32, H as u32),
                )
                .unwrap()
                .into_drawing_area();
                {
                    let mut chart = cs.clone().restore(&root);
                    chart.plotting_area().fill(&BLACK).unwrap();
                    chart
                        .configure_mesh()
                        .bold_line_style(&GREEN.mix(0.2))
                        .light_line_style(&TRANSPARENT)
                        .draw()
                        .unwrap();
                    chart.draw_series(data.iter().zip(data.iter().skip(1)).map(
                        |(&(e, x0, y0), &(_, x1, y1))| {
                            PathElement::new(
                                vec![(x0, y0), (x1, y1)],
                                &GREEN.mix(((e - epoch) * 20.0).exp()),
                            )
                        },
                    )).unwrap();
                }
                root.present().unwrap();
                let keys = window.get_keys_pressed(KeyRepeat::Yes);
                for key in keys {
                    let old_fx = fx;
                    let old_fy = fy;
                    match key {
                        Key::Equal => fy += 0.1,
                        Key::Minus => fy -= 0.1,
                        Key::Key0 => fx += 0.1,
                        Key::Key9 => fx -= 0.1,
                        _ => continue
                    }
                    xphase += 2.0 * epoch * std::f64::consts::PI * (old_fx - fx);
                    yphase += 2.0 * epoch * std::f64::consts::PI * (old_fy - fy);
                    window.set_title(&get_window_title(fx, fy, yphase - xphase));
                }
            }
            window.update_with_buffer(buf.borrow(), W, H).unwrap();
            last_flushed = epoch;
        }
        while let Some((e, _, _)) = data.front() {
            if ((e - epoch) * 20.0).exp() > 0.1 {
                break;
            }
            data.pop_front();
        }
    }
}

fn print_help() {
    println!("Usage: cargo run plotters_test [basic, cpu, minifb]");
}

fn main() {

    match env::args().collect::<Vec::<_>>().last() {
        Some(arg) => match arg.as_str() {
            "basic" => basic_plotting_demo(),
            "cpu" => cpu_plot(),
            "minifb" => minifb_plot(),
            _ => print_help(),
        },
        None => {
            print_help()
        }
    }

}
