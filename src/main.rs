mod linkspsql;
use linkspsql::*;
extern crate tokio;
#[tokio::main]
async fn main() -> Result<(), Error> {
    let (client, connection) = tokio_postgres::connect(
        "", 
        NoTls).await.expect("");
    let link = LinksPSQL::new(client).expect("");
    tokio::spawn(async move {
        if let Err(_) = connection.await { 
            eprintln!("can`t connect"); 
        } 
    });
    let sub = [2;3];
    link.create(&sub).await.expect("");
    Ok(())
}