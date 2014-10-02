#ifndef OSZTALYOK_H
#define OSZTALYOK_H

/* Nem így fogjuk csinálni, hanem teljesen máshogy. Ezeket a classokat szinte biztos, hogy nem fogjuk használni..
 * De azért kösz!


 * Ez egy ideglenes fajl amibe kicsit strukturaltabban
 * osszeszedjuk, hogy milyen objektumokat tervezunk megvalositani
 *
 * TODO szetszedni darabokra
 * TODO tagfugvenyeket megirni
 *
 * esetleg erdemes lehet egy osztaly strukturat kozosen kitalalni
 *

class item //mindennek az ose == minden item == az egesz terkep 1 item
{
protected:
    std::string rovid_leiras;
    std::string hosszu_leiras;

    void IsVegso();
        // ebbe mar nem lehet pakolni = nem kontener
        // hidd el ez majd hasznos lesz

public:
    item();                 //valami ures cucc
    item(std::string sor);  //egy fajl egy sora
    item(std::istream &in); //egy megnyitott fajl
    virtual ~item(); //ez az oroklodes miatt kell. NE TOROLD KI

    void megjelenit (std::ostream &out); //a fuggveny ami szepen kiirja hogy mi is van elottunk

}

class kontener_item: public item
// majdnem minden kontener mert mas itemek vannak benne.
// pl a szoba is kontener mert van benne fal meg lada es persze a fal meg a lada is kontener
// mert abban meg gyuru meg kard meg kep van mik mar valoszinuleg nem kontenerek hacsak nincs a kep moge rejtve valami
{
    vector<item*> kontener;

    void kinyit(std::ostream &out); //mi van elotted amikor eloszor meglatod (pl a ladat kinyitod, vagy a szobaba belesz es meglatod a falat.)
    item* kivesz(unsigned vector_index);
    item* kivesz(item* ezt_vedd_ki);
    void betesz(item & ezt_tedd_be);
}






 #endif // OSZTALYOK_H

