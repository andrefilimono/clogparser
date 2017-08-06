import clogparser
import unittest


class IcecastParserTestCase(unittest.TestCase):
    def test_standart(self):
        expected = ['10.10.10.10', '-', '-', '25/Jul/2017:03:15:22 +0300',
                    'GET /client/stream HTTP/1.0', '200', '66520', '-', 'Winamp 2.81', '0']
        line = '10.10.10.10 - - [25/Jul/2017:03:15:22 +0300] "GET /client/stream HTTP/1.0"' \
               '200 66520 "-" "Winamp 2.81" 0'
        self.assertEqual(
            clogparser.parse_icecast(line),
            expected
        )

    def test_with_inches_in_useragent(self):
        expected = ['10.10.10.10', '-', '-', '25/Jul/2017:03:15:22 +0300',
                    'GET /client/stream_mp3_192kbps HTTP/1.0', '200', '70520', '-',
                    'Mozilla/5.0 (Linux; Android 4.4.2; DEXP Ixion P 4" Build/KOT49H) AppleWebKit/537.36' \
                    '(KHTML, like Gecko) Chrome/56.0.2924.87 Mobile Safari/537.36', '1']
        line = '10.10.10.10 - - [25/Jul/2017:03:15:22 +0300] "GET /client/stream_mp3_192kbps HTTP/1.0" 200 70520 ' \
               '"-" "Mozilla/5.0 (Linux; Android 4.4.2; DEXP Ixion P 4" Build/KOT49H) AppleWebKit/537.36' \
               '(KHTML, like Gecko) Chrome/56.0.2924.87 Mobile Safari/537.36" 1'
        self.assertEqual(
            clogparser.parse_icecast(line),
            expected
        )

    def test_with_two_double_quotes_in_useragent(self):
            expected = ['10.10.10.10', '-', '-', '25/Jul/2017:03:15:22 +0300',
                'GET /client/stream HTTP/1.0', '200', '66520', '-', 'Winamp 2.81', '0']
            line = '10.10.10.10 - - [25/Jul/2017:03:15:22 +0300] "GET /client/stream HTTP/1.0"' \
                '200 66520 "-" ""Winamp 2.81"" 0'
            self.assertEqual(
                clogparser.parse_icecast(line),
                expected
            )

    def test_with_two_double_quotes_and_inches_in_useragent(self):
        expected = ['10.10.10.10', '-', '-', '25/Jul/2017:03:15:22 +0300',
            'GET /client/stream HTTP/1.0', '200', '66520', '-', 'Android 4" Chrome', '0']
        line = '10.10.10.10 - - [25/Jul/2017:03:15:22 +0300] "GET /client/stream HTTP/1.0"' \
            '200 66520 "-" ""Android 4" Chrome" 0'
        self.assertEqual(
            clogparser.parse_icecast(line),
            expected
        )

    def test_invalid(self):
        self.assertEqual(
            clogparser.parse_icecast(''),
            None
        )

        self.assertEqual(
            clogparser.parse_icecast('a a'),
            None
        )

if __name__ == '__main__':
    unittest.main()
