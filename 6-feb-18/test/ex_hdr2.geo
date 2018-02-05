lc1 =25;
lc2 =15;
lc3 =20;
//+
Point (0) ={0   ,0   ,0 , lc1 };
Point (1) ={500 ,0   ,0 , lc1 };
Point (2) ={500 ,500 ,0 , lc1 };
Point (3) ={0   ,500 ,0 , lc1 };
Point (4) ={150 ,200 ,0 , lc2 };
Point (5) ={150 ,300 ,0 , lc2 };
Point (6) ={350 ,200 ,0 , lc2 };
Point (7) ={350 ,300 ,0 , lc2 };
Point (8) ={250 ,250 ,0 , lc3 };
//+
Line (1) ={0 ,1};
Line (2) ={1 ,2};
Line (3) ={2 ,3};
Line (4) ={3 ,0};
Line (5) ={4 ,5};
Line (6) ={6 ,7};
//+
Line Loop (1) ={1 ,2 ,3 ,4};
Surface (1) = {1};
//+
Point{8} In Surface {1};
Line {5} In Surface {1};
Line {6} In Surface {1};
//+
Physical Surface ("unit") = {1};
Physical Line ("fracture") = {5, 6};
Physical Line("circum") = {4, 3, 2, 1};
