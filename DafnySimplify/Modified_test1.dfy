method Main() {
  var i := 0;
  while i < 2
  //invariant 0 <= i
  decreases  2 - i
  {
    i := i + 1;
  }
  //assert 1 < 2;
  //assert 2 < 3;
  //assert 3 < 4;
}
