from unittest import TestCase

import Node


class TestNode(TestCase):
    def test_addNext(self):
        node = Node.Node()
        node.setValue('a')
        self.assertEquals(1, node.getCount())
        self.assertFalse(node.hasNext())
        node.addNext('b')
        self.assertTrue(node.hasNext())
        self.assertEquals('b', node.getNextByValue('b').getValue())

        node.addNext('b')
        self.assertEquals(2, node.getNextByValue('b').getCount())

        node.addNext('a')
        self.assertEquals('a', node.getNextByValue('a').getValue())
