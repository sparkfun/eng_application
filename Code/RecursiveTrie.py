from Node import Node


class RecursiveTrie:
    """
    A data structure used to sort and store data on the frequency of various n-grams and which n-grams occur frequently
    together.  Data is stored in a tree, similar to a traditional Trie, but with frequencies counted at each node, instead
    of at the node at which a string terminates (a leaf).  In addition, each terminal substring (those that begin at various
    postions, but continue until the end of the string) is inserted into the Trie so that the frequencies of all n-grams
    instead of just complete strings.  At the root level, the counts for each node represent the frequencies for each character
    within the sample.  At the first level below root, the frequencies for all strings of length 2 are represented.  In
    general, at level n (with root being 0) the frequencies for all strings of length n + 1 are represented.  The comparative
    frequencies of two n-grams can be extracted by finding the frequency of the first n-gram from root, and then finding the
    second n-gram below the first in that branch of the tree.
    """

    def __init__(self):
        """
        Constructor
        """
        self.root = Node()
        self.delimiters = []

    def getRoot(self):
        """
        Accessor for root
        :return:
        Node root
        """
        return self.root

    def addDelimiter(self, delimiter):
        """
        Add a new delimiter
        :param delimiter:
        String
        """
        self.delimiters.append(delimiter)

    def pushString(self, string):
        """
        Insert a new string into the structure from root
        :param string:
        String string
        """
        formattedString = string
        for i in range(len(self.delimiters)):
            if formattedString.find(self.delimiters[i]) > 0:
                formattedString = formattedString[:formattedString.find(self.delimiters[i])]
        self.getRoot().addNext(formattedString)

    def getCountByString(self, string):
        """
        Returns the frequency of the given string from root.
        :param string:
        String
        :return:
        int count
        """
        traverse = self.getRoot()
        for i in range(len(string)):
            traverse = traverse.getNextByValue(string[i])
            if traverse == None:
                return 0
        return traverse.getCount()

    def getDirectedRelativeFrequency(self, firstString, secondString):
        """
        Find the frequency of secondString where it follows firstString
        :param first:
        String
        :param second:
        String
        :return:
        int count
        """
        formattedString = firstString + secondString
        return self.getCountByString(formattedString)

    def getSymmetricFrequency(self, firstString, secondString):
        """
        find the frequency of firstString and secondString when they occur together regardless of the order in which
        they occur
        :param firstString:
        String
        :param secondString:
        String
        :return:
        int count
        """
        string1 = firstString + secondString
        string2 = secondString + firstString
        return self.getCountByString(string1) + self.getCountByString(string2)