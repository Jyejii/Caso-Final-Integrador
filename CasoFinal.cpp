#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <nlohmann/json.hpp>


enum variant_type { Symbol, Number, List, Proc, Lambda, Cadena };

struct Entorno;

class Variant {
public:
    typedef Variant(*proc_type)(const std::vector<Variant>&);
    typedef std::vector<Variant>::const_iterator iter;
    typedef std::map<std::string, Variant> map;

    variant_type type;
    std::string val;
    std::vector<Variant> list;
    proc_type proc;
    Entorno* env;

    Variant(variant_type type = Symbol) : type(type), env(0), proc(0) {}
    Variant(variant_type type, const std::string& val) : type(type), val(val), env(0), proc(0) {}
    Variant(proc_type proc) : type(Proc), proc(proc), env(0) {}

    std::string to_string();
    std::string to_json_string();
    static Variant from_json_string(const std::string& json);
    static Variant parse_json(const json11::Json& json); // Asegúrate de que esta es la definición correcta de la función
};

std::string Variant::to_string() {
    switch (type) {
        case Symbol:
            return val;
        case Number:
            return std::to_string(std::stoi(val)); // Cambiar a std::stod() si se espera un double
        case List: {
            std::string result = "[";
            for (const Variant& v : list) {
                result += v.to_string() + ", ";
            }
            if (!list.empty()) {
                result.pop_back();
                result.pop_back();
            }
            result += "]";
            return result;
        }
        case Proc:
            return "Procedure";
        case Lambda:
            return "Lambda";
        case Cadena:
            return "\"" + val + "\"";
        default:
            return "Unknown Type";
    }
}

std::string Variant::to_json_string() {
    json11::Json::array json_list;
    switch (type) {
        case Symbol:
            return "{\"type\": \"Symbol\", \"value\": \"" + val + "\"}";
        case Number:
            return "{\"type\": \"Number\", \"value\": " + val + "}";
        case List: {
            for (const Variant& v : list) {
                json_list.push_back(v.to_json_string());
            }
            return "{\"type\": \"List\", \"value\": " + json11::Json(json_list).dump() + "}";
        }
        case Proc:
            return "{\"type\": \"Proc\"}";
        case Lambda:
            return "{\"type\": \"Lambda\"}";
        case Cadena:
            return "{\"type\": \"Cadena\", \"value\": \"" + val + "\"}";
        default:
            return "{\"type\": \"Unknown\"}";
    }
}

Variant Variant::from_json_string(const std::string& sjson) {
    std::string err;
    auto json = json11::Json::parse(sjson, err);
    if (!err.empty()) {

        return Variant();
    }

    std::string type = json["type"].string_value();
    if (type == "Symbol") {
        return Variant(Symbol, json["value"].string_value());
    } else if (type == "Number") {
        return Variant(Number, std::to_string(json["value"].number_value()));
    } else if (type == "List") {
        auto array = json["value"].array_items();
        Variant variantList(List);
        for (const auto& item : array) {
            variantList.list.push_back(from_json_string(item.dump()));
        }
        return variantList;
    } else if (type == "Cadena") {
        return Variant(Cadena, json["value"].string_value());
    } else if (type == "Proc") {
        return Variant([](const std::vector<Variant>&) {

            return Variant();
        });
    } else {

        return Variant();
    }
}

Variant Variant::parse_json(const json11::Json& json) {

    return Variant();
}

int main() {
    // Ejemplo de uso
    Variant symbolVariant(Symbol, "Hello");
    Variant numberVariant(Number, "42");
    Variant listVariant(List);
    listVariant.list.emplace_back(Symbol, "apple");
    listVariant.list.emplace_back(Symbol, "orange");
    listVariant.list.emplace_back(Number, "123");

    std::cout << "to_string() Example:\n";
    std::cout << "Symbol Variant: " << symbolVariant.to_string() << std::endl;
    std::cout << "Number Variant: " << numberVariant.to_string() << std::endl;
    std::cout << "List Variant: " << listVariant.to_string() << std::endl;

    std::cout << "\nto_json_string() Example:\n";
    std::cout << "Symbol Variant JSON: " << symbolVariant.to_json_string() << std::endl;
    std::cout << "Number Variant JSON: " << numberVariant.to_json_string() << std::endl;
    std::cout << "List Variant JSON: " << listVariant.to_json_string() << std::endl;


    std::string jsonString = "{\"type\": \"List\", \"value\": [{\"type\": \"Symbol\", \"value\": \"apple\"}, {\"type\": \"Symbol\", \"value\": \"orange\"}, {\"type\": \"Number\", \"value\": 123}]}";
    Variant fromJson = Variant::from_json_string(jsonString);
    std::cout << "\nfrom_json_string() Example:\n";
    std::cout << "Variant from JSON: " << fromJson.to_string() << std::endl;



    return 0;
}
