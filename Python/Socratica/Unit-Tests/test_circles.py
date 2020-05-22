'''
Unit test for circles.py

To run a unit test in the shell, use the following command:
    python3 -m unittest Test_circles

@author: Matt Marti
@date: 2019-06-07
'''

import unittest;
from circles import circle_area;
from math import pi;

class test_CircleArea(unittest.TestCase):
    
    # Test areas when radius >= 0
    def test_area(self):
        self.assertAlmostEqual(circle_area(1), pi);
        self.assertAlmostEqual(circle_area(0), 0);
        self.assertAlmostEqual(circle_area(2.1), pi*2.1**2);
    #
    
    # Make sure value errors are raised for bad numbers
    def test_values(self):
        self.assertRaises(ValueError, circle_area, -2);
    #
    
    # Make sure t
    def test_types(self):
        self.assertRaises(TypeError, circle_area, 3+5j);
        self.assertRaises(TypeError, circle_area, True);
        self.assertRaises(TypeError, circle_area, "Radius");
    #
#