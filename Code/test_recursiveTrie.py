from unittest import TestCase

from RecursiveTrie import RecursiveTrie


class TestRecursiveTrie(TestCase):
    def test_pushString(self):
        trie = RecursiveTrie()
        trie.pushString('trie')
        root = trie.getRoot()
        next = root.getNextByValue('t')
        self.assertTrue(next != None)
        next = next.getNextByValue('r')
        self.assertTrue(next != None)
        next = next.getNextByValue('i')
        self.assertTrue(next != None)
        next = next.getNextByValue('e')
        self.assertTrue(next != None)
        self.assertEquals(1, next.getCount())

    def test_delimiters(self):
        trie = RecursiveTrie()
        trie.addDelimiter('.')
        trie.pushString("a.b")
        root = trie.getRoot()
        next = root.getNextByValue('a')
        next = next.getNextByValue('.')
        self.assertEquals(next, None)

    def test_getCountByString(self):
        trie = RecursiveTrie()
        trie.pushString('trie')
        trie.pushString('trial')
        self.assertEquals(2, trie.getCountByString('tri'))
        self.assertEquals(1, trie.getCountByString('trie'))

    def test_getDirectedRelativeFrequency(self):
        trie = RecursiveTrie()
        trie.pushString('abcd')
        trie.pushString('cdab')
        count = trie.getDirectedRelativeFrequency('ab', 'cd')
        self.assertEquals(1, count)

    def test_getSymmetricFrequency(self):
        trie = RecursiveTrie()
        trie.pushString('abcd')
        trie.pushString('cdab')
        count = trie.getSymmetricFrequency('ab', 'cd')
        self.assertEquals(2, count)
