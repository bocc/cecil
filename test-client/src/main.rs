use std::{
    io::{Read, Write},
    net::{Shutdown, SocketAddr, TcpStream},
    str::FromStr,
    time::Duration,
};

fn main() -> std::io::Result<()> {
    let handles = (1..10)
        .map(|_| std::thread::spawn(test_client))
        .collect::<Vec<_>>();

    for h in handles.into_iter() {
        match h.join() {
            Ok(res) => assert!(res.is_ok(), res),
            Err(e) => assert!(false, e),
        }
    }

    Ok(())
}

fn test_client() -> std::io::Result<()> {
    let mut buf: [u8; 200] = [0; 200];
    let testcases = vec![
        (b"1234\n".to_vec(), b"4321".to_vec()),
        (b"1234\n4567\n89\n".to_vec(), b"4321765498".to_vec()),
        (b"1234\n4567\n\n\n89\n".to_vec(), b"4321765498".to_vec()),
    ];

    let socket = SocketAddr::from_str("127.0.0.1:8888").expect("invalid address");
    let mut stream = TcpStream::connect_timeout(&socket, Duration::from_secs(2))?;

    for (tc, expected) in testcases.into_iter() {
        stream.write_all(&tc)?;

        // this is questionable
        std::thread::sleep(Duration::from_millis(100));

        let n = stream.read(&mut buf)?;

        let expected = String::from_utf8(expected.to_vec()).unwrap();
        let received = String::from_utf8(buf[..n].to_vec()).unwrap();
        assert_eq!(expected, received);

        println!("expected: {}, got: {}", expected, received);
    }

    stream.shutdown(Shutdown::Both)
}
