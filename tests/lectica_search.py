import unittest
import doctest
import lectica


class SearchTest(unittest.TestCase):
    def runTest(self):
        try:
            import lectica
        except ImportError as e:
            self.Fail(str(e))

    def test_search_for_terms(self):
        text = """Approach the inquiry with an attitude of curiosity and wonder recognize that a perfect algorithm is
               unlikely to exist and that this process is ongoing and iterative. """
        terms = ["inquiry", "ongoing", "petrukio", "jorel"]

        terms_found = lectica.search_terms_in_text(text, terms)
        self.assertListEqual( terms_found , ["inquiry", "ongoing"])

    def test_search_for_terms_next_to_punctuation(self):
        text = """Approach the inquiry with an attitude of curiosity and wonder, recognize that a perfect algorithm is
               unlikely to exist and that this process is ongoing and iterative. Another use case; for this."""
        terms = ["inquiry", "ongoing", "petrukio", "jorel", "iterative", "wonder", "case"]

        terms_found = lectica.search_terms_in_text(text, terms)
        self.assertListEqual( sorted(terms_found) , sorted(["iterative", "inquiry", "ongoing", "wonder", "case"]))

    def test_search_terms_with_hyphens(self):
        text = """Approach the inquiry with an attitude of endeavor-- of curiosity and --beautiful wonder, recognize that a perfect algorithm is
               unlikely to exist and that this process is ongoing and iterative. Another use case; for this."""
        terms = ["inquiry", "ongoing", "petrukio", "jorel", "iterative", "wonder", "cases", "endeavor", "beautiful"]

        terms_found = lectica.search_terms_in_text(text, terms)
        self.assertListEqual(sorted(terms_found), sorted(["iterative", "inquiry", "ongoing", "wonder", "beautiful", "endeavor"]))

    def test_search_terms_with_hyphens_two_words(self):
        text = "Do I care about this issue--deeply? "
        terms = ["deeply", "ongoing", "petrukio", "jorel", "iterative", "wonder", "cases", "endeavor", "beautiful"]

        terms_found = lectica.search_terms_in_text(text, terms)
        self.assertListEqual(sorted(terms_found), sorted(["deeply"]))

    def test_search_terms_with_periods_followed_by_commas(self):
        text = """Approach the inquiry with an attitude of endeavor-- of curiosity and --beautiful wonder, recognize that a perfect algorithm is
                unlikely to exist and that this process is ongoing and iterative., another,. use case; for this."""
        terms = ["inquiry", "ongoing", "petrukio", "jorel", "iterative", "wonder", "another", "endeavor", "beautiful"]

        terms_found = lectica.search_terms_in_text(text, terms)
        self.assertListEqual(sorted(terms_found),
                             sorted(["iterative", "inquiry", "ongoing", "wonder", "beautiful", "endeavor", "another"]))