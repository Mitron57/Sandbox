pub extern crate tokio_postgres;

pub use tokio_postgres::{Client, NoTls, tls::NoTlsStream, Error, Statement};

pub struct LinksPSQL {
    pub client: Client,
    //pub connection: Connection<Socket, NoTlsStream> //maybe we don`t need this
}
impl LinksPSQL {
    pub fn new(client: Client) -> Result<Self, Error> {
        //let connection = tokio_postgres::connect(dbopts, NoTls).await?.1;
        Ok(Self { client })
    }
    
    pub async fn create(&self, substitution: &[i32]) -> Result<Statement, Error> {
        self.client.prepare(&format!(
            "INSERT INTO Links VALUES ({}, {}, {});", 
            substitution[0], substitution[1], substitution[2])).await
    }
    
    /*pub async fn count(&self, restriction: &[i32]) -> i32 {
        let result = self.client.prepare(&format!("SELECT (*) COUNT FROM Links WHERE id = {}", restriction[0])).await?;
    }*/
    pub async fn delete(&self, restriction: &[i32]) -> Result<Statement, Error> {
        self.client.prepare(&format!("DELETE FROM Links WHERE id = {}", restriction[0])).await
    }
}