active proctype p (){
    byte i;
    select(i: 0..10);
    assert(i > 0);
}
