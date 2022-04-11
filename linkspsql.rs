pub extern crate tokio_postgres;
pub use tokio_postgres::{*, Client, Connection, NoTls, Socket, tls::NoTlsStream};

pub struct LinksPSQL {
    pub client: Client,
    pub connection: Connection<Socket, NoTlsStream>
}
impl LinksPSQL {
    pub async fn new(dbopts: &str) -> Result<Self, Box<dyn std::error::Error>> {
        let (client, connection) = tokio_postgres::connect(dbopts, NoTls).await?;
        Ok(Self {
            client,
            connection,
        })
    }
}

#[cfg(test)]
mod test {
    use super::LinksPSQL;

    #[tokio::test]
    async fn test() {
        let opts = "your_db_params";
        assert!(match LinksPSQL::new(opts).await {
            Ok(_) => true,
            Err(_) => false,
        }); 
    }
}