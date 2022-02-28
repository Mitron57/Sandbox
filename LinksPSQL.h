#include <pqxx/pqxx>
#include <pqxx/connection_base>

namespace Platform::Data::Doublets
{
    template<typename TLink>
    struct LinksPSQL //: public ILinks<LinksPSQL<TLink>, TLink>
    {
        
        auto foo() -> std::string //test
        {
            return c.dbname();
        }
        
        explicit LinksPSQL(const std::string&& dbopts) : opts(dbopts)
        {
            query = "CREATE TABLE IF NOT EXISTS Links(id bigint, from_id bigint, to_id bigint);";
            w.exec(query);
        }
        
        virtual ~LinksPSQL()
        {
            w.commit();
            c.disconnect();
        }
        
        auto Exists(auto&& link) -> bool
        {
            pqxx::result r = w.exec("SELECT * FROM Links WHERE id = " + std::to_string(link.Index) + ";");
            if(r[0][0].c_str()=="")
                return false;
            else
                return true;
        }
        
        auto Create() -> void
        {
            Link<TLink> link(0, 0, 0);
            query = "INSERT INTO Links VALUES (" + std::to_string(link.Index) + ", " 
            + std::to_string(link.Source) + ", " + std::to_string(link.Target) + ");";
            w.exec(query);
        }
        
        auto Update(const Link<TLink>& link) -> void
        {
            query = "UPDATE Links SET from_id = " + std::to_string(link.Source) + ", to_id = "
            + std::to_string(link.Target) + " WHERE id = " + std::to_string(link.Index) + ";";
            w.exec(query);
        }
        
        auto Delete(auto&& restrictions) -> void
        {
            if(!this->Exists(restrictions[0]))
                std::cerr<<"You can`t delete non-existent link.";
            else
            {
                query = "DELETE FROM Links WHERE id = " + std::to_string(restrictions) + ";";
                w.exec(query);
            }
        }
        
        auto Count(auto&& restrictions) -> std::int32_t
        {
            query = "SELECT COUNT(*) FROM Links WHERE id = " + std::to_string(restrictions[0]) + ";";
            pqxx::result r = w.exec(query);
            return std::stoi(r[0][0].c_str());
        }
        
        auto Each(auto&& restrictions) /*requires requires {std::integral<TLink>;}*/
        {
            LinksConstants<TLink> constants;
            if (restrictions[0] == constants.Any && restrictions[1] == constants.Any && restrictions[2] == constants.Any)
                query = "SELECT * FROM Links;";
            if (restrictions[0] != constants.Any && restrictions[1] == constants.Any && restrictions[2] == constants.Any)
                query = "SELECT * FROM Links WHERE id = " + std::to_string(restrictions[0]) + ";";
            if (restrictions[0] == constants.Any && restrictions[1] != constants.Any && restrictions[2] == constants.Any)
                query = "SELECT * FROM Links WHERE from_id = " + std::to_string(restrictions[1]) + ";";
            if (restrictions[0] == constants.Any && restrictions[1] == constants.Any && restrictions[2] != constants.Any)
                query = "SELECT * FROM Links WHERE to_id = " + std::to_string(restrictions[2]) + ";";
            if (restrictions[0] == constants.Any && restrictions[1] != constants.Any && restrictions[2] != constants.Any)
                query = "SELECT * FROM Links WHERE from_id = " + std::to_string(restrictions[1]) + "AND to_id = " + std::to_string(restrictions[2]) + ";";
            pqxx::result r = w.exec(query);
            for(int i{}; i<r.size(); i++)
            {
                for(int j{}; j<r[i].size(); i++)
                {
                    Link<TLink> link(0, 0, 0);
                    int link_param{};
                    for(int a{}; a<3; a++){
                        r[i][j].to(link_param);
                        link[a] = link_param;
                    }
                    std::cout<<link;
                }
            }
            
        }
        
        private: std::string opts{};
        private: std::string query{};
        
        private: pqxx::asyncconnection c{opts};
        private: pqxx::work w{c};
    };
}
