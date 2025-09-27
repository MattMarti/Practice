use std::env;
use std::fs;
use std::collections::VecDeque;
use std::error::Error;
use std::time::SystemTime;
use std::borrow::{Borrow, BorrowMut};
use std::thread::sleep;
use std::time::Duration;

// Minifb tools
use plotters_bitmap::bitmap_pixel::BGRXPixel;
use plotters_bitmap::BitMapBackend;
use minifb::{Key, Window, WindowOptions};

use plotters::prelude::*;

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

fn main() -> Result<(), Box<dyn std::error::Error>> {
    const WIDTH: usize = 800;
    const HEIGHT: usize = 600;

    let mut buf = BufferWrapper(vec![0u32; WIDTH * HEIGHT]);

    {
        let root_area = BitMapBackend::<BGRXPixel>::with_buffer_and_format(
            buf.borrow_mut(), (WIDTH as u32, HEIGHT as u32)
        )
        .unwrap()
        .into_drawing_area();

        root_area.fill(&WHITE);

        let root_area = root_area.titled("Image Title", ("sans-serif", 60))?;

        let x_axis = (-3.4f32..3.4).step(0.1);

        let mut cc = ChartBuilder::on(&root_area)
            .margin(5)
            .set_all_label_area_size(50)
            .caption("Sine and Cosine", ("sans-serif", 40))
            .build_cartesian_2d(-3.4f32..3.4, -1.2f32..1.2f32)?;

        cc.configure_mesh()
            .x_labels(20)
            .y_labels(10)
            .disable_mesh()
            .x_label_formatter(&|v| format!("{:.1}", v))
            .y_label_formatter(&|v| format!("{:.1}", v))
            .draw()?;

        cc.draw_series(LineSeries::new(x_axis.values().map(|x| (x, x.sin())), &RED))?
            .label("Sine")
            .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], RED));

        cc.draw_series(LineSeries::new(
            x_axis.values().map(|x| (x, x.cos())),
            &BLUE,
        ))?
        .label("Cosine")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], BLUE));

        cc.configure_series_labels().border_style(BLACK).draw()?;

        cc.draw_series(PointSeries::of_element(
            (-3.0f32..2.1f32).step(1.0).values().map(|x| (x, x.sin())),
            5,
            ShapeStyle::from(&RED).filled(),
            &|coord, size, style| {
                EmptyElement::at(coord)
                    + Circle::new((0, 0), size, style)
                    + Text::new(format!("{:?}", coord), (0, 15), ("sans-serif", 15))
            },
        ))?;
    };
    let mut window = Window::new(
        "My Plot",
        WIDTH,
        HEIGHT,
        WindowOptions::default()
    ).unwrap();

    //let root = BitMapBackend::<BGRXPixel>::with_buffer_and_format(
    //    buf.borrow_mut(),
    //    (WIDTH as u32, HEIGHT as u32),
    //)
    //.unwrap()
    //.into_drawing_area();

    while window.is_open() && !window.is_key_down(Key::Escape) {
        window.update_with_buffer(buf.borrow(), WIDTH, HEIGHT).unwrap();
        sleep(Duration::from_millis(50));
    }

    Ok(())
}
