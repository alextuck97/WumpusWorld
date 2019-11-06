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

    def tearDown(self):
        pass