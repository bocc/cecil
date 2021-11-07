use std::{
    io::{Read, Write},
    net::TcpStream,
};

fn main() -> std::io::Result<()> {
    let mut buf: [u8; 200] = [0; 200];
    let testcases = vec![b"1234\n".to_vec(), b"1234\n4567\n89\n".to_vec()];

    let mut stream = TcpStream::connect("localhost:8888")?;

    for tc in testcases.into_iter() {
        stream.write_all(&tc)?;

        for s in tc.split(|&c| c == b'\n').filter(|&s| !s.is_empty()) {
            println!("read {:?}", &s);
            let n = stream.read(&mut buf)?;

            let mut s = s.to_vec();
            s.reverse();

            let sent = String::from_utf8(s).unwrap();
            let received = String::from_utf8(buf[..n].to_vec()).unwrap();

            println!("sent: {}, got: {}", sent, received);
        }
    }

    Ok(())
}
