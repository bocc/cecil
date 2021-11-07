use std::{
    io::{Read, Write},
    net::TcpStream,
};

fn main() -> std::io::Result<()> {
    test_client()
}

fn test_client() -> std::io::Result<()> {
    let mut buf: [u8; 200] = [0; 200];
    let testcases = vec![
        (b"1234\n".to_vec(), b"4321".to_vec()),
        (b"1234\n4567\n89\n".to_vec(), b"4321\n7654\n98\n".to_vec()),
        (b"1234\n4567\n\n\n89\n".to_vec(), b"4321\n7654\n98\n".to_vec()),
    ];

    let mut stream = TcpStream::connect("localhost:8888")?;

    for (tc, expected) in testcases.into_iter() {
        stream.write_all(&tc)?;

        for expected in expected.split(|&c| c == b'\n').filter(|&s| !s.is_empty()) {
            let n = stream.read(&mut buf)?;

            let expected = String::from_utf8(expected.to_vec()).unwrap();
            let received = String::from_utf8(buf[..n].to_vec()).unwrap();

            println!("expected: {}, got: {}", expected, received);
        }
    }

    Ok(())
}
