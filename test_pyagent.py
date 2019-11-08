import PyAgent as pya
import unittest

class TestPyAgent(unittest.TestCase):

    def setUp(self):
        self.agent = pya.Agent()

    def test_update_state(self):
        self.agent.update_state(True, True, False, False,False)
        stench = [(1,1)]
        breeze = [(1,1)]
        frontier = set([(2,1), (1,2)])
        known = [(1,1)]
        self.assertEqual(set(self.agent.frontier), frontier)
        self.assertEqual(self.agent.breezes, breeze)
        self.assertEqual(self.agent.stenches, stench)
        self.assertEqual(self.agent.known[0], known)

        self.agent.update_location()
        self.agent.update_state(False, True,False,False,False)
        
        breeze = set([(1,1), (2,1)])
        known = set([(1,1), (2,1)])
        frontier = set([(1,2), (2,2), (3,1)])
        self.assertEqual(set(self.agent.stenches), set(stench))
        self.assertEqual(set(self.agent.breezes), breeze)
        self.assertEqual(set(self.agent.frontier), frontier)
        self.assertEqual(set(self.agent.frontier), frontier)


    def test_breeze_consistency(self):
        #Random tests. Seems to work
        breeze = [(2,2), (2,3), (4,5)]
        frontier = [(1,2), (2,4),(5,5)]
        C0 = '000'
        C1 = '010'
        C2 = '101'
        C3 = '111'
        testing_pit = (1,5)
        self.assertEqual(self.agent.is_breeze_consistent(breeze, frontier,C0), False)
        self.assertEqual(self.agent.is_breeze_consistent(breeze, frontier,C0, testing_pit), False)
        self.assertEqual(self.agent.is_breeze_consistent(breeze, frontier, C2), False)
        self.assertEqual(self.agent.is_breeze_consistent(breeze, frontier, C3), True)
        self.assertEqual(self.agent.is_breeze_consistent(breeze,frontier,C1, testing_pit), False)
        
    def test_pit_probability(self):
        frontier = [(3,1),(1,3),(2,2)]
        breezes = [(1,2),(2,1)]
        expected = {(3,1):0.31,(2,2):0.86,(1,3):0.31}
        self.agent.calculate_pits(frontier, breezes, {})
        self.assertAlmostEqual(self.agent.pit_probabilities, expected)
        

    def tearDown(self):
        pass

if __name__ == "__main__":
    unittest.main()