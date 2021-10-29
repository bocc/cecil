use std::net::TcpStream;

fn main() {
    // let addr = SocketAddr::from(([127, 0, 0, 1], 443));
    if let Ok(_stream) = TcpStream::connect("localhost:8888") {
        println!("connected");
    } else {
        println!("didn't connect");
    }
}
