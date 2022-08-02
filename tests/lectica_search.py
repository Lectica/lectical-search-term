import unittest
import doctest
import lectica


class SearchTest(unittest.TestCase):
    def runTest(self):
        try:
            import lectica
        except ImportError as e:
            self.Fail(str(e))

    def test_searchForTermsTest(self):
        text = """Approach the inquiry with an attitude of curiosity and wonder recognize that a perfect algorithm is
               unlikely to exist and that this process is ongoing and iterative. """
        terms = ["inquiry", "ongoing", "petrukio", "jorel"]

        terms_found = lectica.search_terms_in_text(text, terms)
        self.assertListEqual( terms_found , ["inquiry", "ongoing"])

    def test_searchForTermsTestNextToPunctuation(self):
        text = """Approach the inquiry with an attitude of curiosity and wonder, recognize that a perfect algorithm is
               unlikely to exist and that this process is ongoing and iterative. Another use case; for this."""
        terms = ["inquiry", "ongoing", "petrukio", "jorel", "iterative", "wonder", "case"]

        terms_found = lectica.search_terms_in_text(text, terms)
        self.assertListEqual( sorted(terms_found) , sorted(["iterative", "inquiry", "ongoing", "wonder", "case"]))
