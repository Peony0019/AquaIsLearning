use plotpy::{self, linspace, Curve};
use polars::prelude::*;
use rand::prelude::*;

fn main() {
    println!("{}", DataFrame::empty());
}

fn series2graph(series: &Series) -> plotpy::GraphMaker {
    let n = series.len();
    let x = linspace(0., 1., n);
    let y = series.into();

    return Curve(&x, &y, "line", "blue");
}

fn read4web() {
    // This function reads data from a web API and returns a DataFrame
    // It's a placeholder and doesn't actually perform the web request
    let plot: plotpy::Plot = plotpy::Plot::new();
    let rvs: [f32; 32] = rand::thread_rng().gen();
    let rv_series: Series = Series::new("col".into(), rvs);

    plot.add(&rv_series);

}





