#include <pqxx/pqxx>

namespace Platform::Data::Doublets
{
    template<typename TLink>
    struct LinksPSQL: public ILinks<LinksPSQL<TLink>, TLink>
    {
        
        auto foo() -> std::string //test
        {
            return c.dbname();
        }
        
        explicit LinksPSQL(const std::string& dbopts) : opts(dbopts)
        {
            query = "CREATE TABLE IF NOT EXISTS Links(id bigint, from_id bigint, to_id bigint);";
            w.exec(query);
        }
        
        virtual ~LinksPSQL()
        {
            w.commit();
            c.close();
        }
        
        auto Exists(auto&& Index) -> bool
        {
            pqxx::result r = n.exec("SELECT * FROM Links WHERE id = " + std::to_string(Index) + ";");
            if(r[0][0].c_str()=="")
                return false;
            else
                return true;
        }
        
        auto Create(Interfaces::CArray auto&& substitutions) -> void
        {
            query = "SELECT * FROM Links;";
            pqxx::result r = n.exec(query);
            query = "INSERT INTO Links VALUES (" + std::to_string(1) + ", " 
            + std::to_string(substitutions[0]) + ", " + std::to_string(substitutions[1]) + ")";
            w.exec(q);
        }
        
        auto Update(Interfaces::CArray auto&& restrictions, Interfaces::CArray auto&& substitutions) -> void
        {
            if (std::size(restrictions)==1 || std::size(restrictions)==3)
            {
                query = "UPDATE Links SET from_id = " + std::to_string(substitutions[0]) + ", to_id = "
                + std::to_string(substitutions[1]) + " WHERE id = " + std::to_string(restrictions[0]) + ";";  
            }
            if(std::size(restrictions)==2)
            {
                query = "UPDATE Links SET from_id = " + std::to_string(substitutions[0]) + ", to_id = "
                + std::to_string(substitutions[1]) + " WHERE from_id = " + std::to_string(restrictions[0])
                + "AND to_id = " + std::to_string(restrictions[1]) + ";";
            }
            w.exec(query);
        }
        
        auto Delete(Interfaces::CArray auto&& restrictions) -> void
        {
            if(!this->Exists(restrictions[0]))
                std::cerr<<"You can`t delete non-existent link.";
            else
            {
                query = "DELETE FROM Links WHERE id = " + std::to_string(restrictions[0]) + ";";
                w.exec(query);
            }
        }
        
        auto Count(Interfaces::CArray auto&& restrictions) -> int
        {
            LinksConstants<TLink> constants;
            if (restrictions[0] == constants.Any && restrictions[1] == constants.Any && restrictions[2] == constants.Any)
                query = "SELECT COUNT(*) FROM Links;";
            if (restrictions[0] != constants.Any && restrictions[1] == constants.Any && restrictions[2] == constants.Any)
                query = "SELECT COUNT(*) FROM Links WHERE id = " + std::to_string(restrictions[0]) + ";";
            if (restrictions[0] == constants.Any && restrictions[1] != constants.Any && restrictions[2] == constants.Any)
                query = "SELECT COUNT(*) FROM Links WHERE from_id = " + std::to_string(restrictions[1]) + ";";
            if (restrictions[0] == constants.Any && restrictions[1] == constants.Any && restrictions[2] != constants.Any)
                query = "SELECT COUNT(*) FROM Links WHERE to_id = " + std::to_string(restrictions[2]) + ";";
            if (restrictions[0] == constants.Any && restrictions[1] != constants.Any && restrictions[2] != constants.Any)
                query = "SELECT COUNT(*) FROM Links WHERE from_id = " + std::to_string(restrictions[1]) + "AND to_id = " + std::to_string(restrictions[2]) + ";";
            pqxx::result r = w.exec(query);
            return r[0][0].as<int>();
        }
        
        auto Each(Interfaces::CArray auto&& restrictions) -> void /*requires requires {std::integral<TLink>;}*/
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
            Link<TLink> link (0, 0, 0);
            for(int i{}; i<r.size(); i++)
            {
                for(int j{}; j<3; j++)
                {
                    link[j] = r[i][j].as<TLink>();
                }
                std::cout<<link<<'\n';
            }
            
        }
        
        private: std::string opts{};
        private: std::string_view query{};
        
        private: pqxx::connection c{opts};
        private: pqxx::work w{c};
    };
}