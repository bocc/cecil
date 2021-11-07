use std::{io::{Read, Write}, net::TcpStream};

fn main() -> std::io::Result<()> {
    let mut buf: [u8; 200] = [0; 200];
    // let addr = SocketAddr::from(([127, 0, 0, 1], 443));
    if let Ok(mut stream) = TcpStream::connect("localhost:8888") {
        println!("connected");
        stream.write_all(b"1234\n")?;
        let n = stream.read(&mut buf)?;
        let s = String::from_utf8(buf[..n].to_vec()).unwrap();
        println!("got back: n: {} {:?}", n, s);

        Ok(())
    } else {
        println!("didn't connect");
        Ok(())
    }
}
