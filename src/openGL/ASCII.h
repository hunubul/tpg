#ifndef SOURCE_H
#define SOURCE_H

#include <map>
#include <vector>

namespace ASCIICHAR {
	enum CHAR;
}
class FontChar;

const int FontX = 8;
const int FontY = 8;
const int WriteOutFontX = 8;
const int WriteOutFontY = 12;
extern FontChar fontChar;

class FontChar {
private:
public:
	std::map<ASCIICHAR::CHAR, std::vector<std::vector<bool>> > ASCIIFont;
	std::map<ASCIICHAR::CHAR, std::vector<std::vector<bool>> > ASCIIFontWriteOut;
	std::vector<unsigned char> ASCIIChar;
	std::vector<unsigned char> ASCIIBrightness;
	std::map<ASCIICHAR::CHAR, std::vector<unsigned short>> ASCIIHistogram;

	FontChar();
	int size();
};

namespace ASCIICHAR {
	enum CHAR {
		EMPTY = 0,
		SMILE,
		SMILE_INVERTED,
		HEART, DIAMONDS,
		CLUBS, SPADES,
		DOT,
		DOT_INVERTED,
		CIRCLE,
		CIRCLE_DOTTED,
		MALE,
		FEMALE,
		NOTE_QUARTER,
		NOTE_EIGHTH,
		LIGHT,
		ARROW2_RIGHT,
		ARROW2_LEFT,
		ARROW_DOUBLE_VERTICAL,
		EXCLAMATION_DOUBLE,
		PILCROW,
		SECTION,
		BLOCK_HALF_LOWER,
		ARROW_DOUBLE_VERTICAL_LINED,
		ARROW_UP,
		ARROW_DOWN,
		ARROW_RIGHT,
		ARROW_LEFT,
		CORNER_LOWERLEFT,
		ARROW_DOUBLE_HORIZONTAL,
		ARROW2_UP,
		ARROW2_DOWN,
		SPACE,
		EXCLAMATION,
		DOUBLE_QUOTE,
		HASHMARK,
		DOLLAR,
		PERCENT,
		AND,
		APOSTROPHE,
		BRACKET_LEFT,
		BRACKET_RIGHT,
		ASTERIX,
		PLUS,
		COMMA,
		DASH,
		PERIOD,
		SLASH,
		NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
		COLON, SEMICOLON,
		LESS_THAN, EQUAL, GREATER_THAN,
		QUESTION, AT,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		SQUARE_BRACKET_LEFT, BACKSLASH, SQUARE_BRACKET_RIGHT, ROOF, UNDERSCORE,
		APOSTROPHE_INVERTED,
		a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
		BRACKETS_CURLY_LEFT, VERTICAL_BAR, BRACKETS_CURLY_RIGHT,
		TILDE, HOUSE,
		EXTENDED_128,
		EXTENDED_129,
		EXTENDED_130,
		EXTENDED_131,
		EXTENDED_132,
		EXTENDED_133,
		EXTENDED_134,
		EXTENDED_135,
		EXTENDED_136,
		EXTENDED_137,
		EXTENDED_138,
		EXTENDED_139,
		EXTENDED_140,
		EXTENDED_141,
		EXTENDED_142,
		EXTENDED_143,
		EXTENDED_144,
		EXTENDED_145,
		EXTENDED_146,
		EXTENDED_147,
		EXTENDED_148,
		EXTENDED_149,
		EXTENDED_150,
		EXTENDED_151,
		EXTENDED_152,
		EXTENDED_153,
		EXTENDED_154,
		EXTENDED_155,
		EXTENDED_156,
		EXTENDED_157,
		EXTENDED_158,
		EXTENDED_159,
		EXTENDED_160,
		EXTENDED_161,
		EXTENDED_162,
		EXTENDED_163,
		EXTENDED_164,
		EXTENDED_165,
		EXTENDED_166,
		EXTENDED_167,
		EXTENDED_168,
		EXTENDED_169,
		EXTENDED_170,
		EXTENDED_171,
		EXTENDED_172,
		EXTENDED_173,
		EXTENDED_174,
		EXTENDED_175,
		EXTENDED_176,
		EXTENDED_177,
		EXTENDED_178,
		EXTENDED_179,
		EXTENDED_180,
		EXTENDED_181,
		EXTENDED_182,
		EXTENDED_183,
		EXTENDED_184,
		EXTENDED_185,
		EXTENDED_186,
		EXTENDED_187,
		EXTENDED_188,
		EXTENDED_189,
		EXTENDED_190,
		EXTENDED_191,
		EXTENDED_192,
		EXTENDED_193,
		EXTENDED_194,
		EXTENDED_195,
		EXTENDED_196,
		EXTENDED_197,
		EXTENDED_198,
		EXTENDED_199,
		EXTENDED_200,
		EXTENDED_201,
		EXTENDED_202,
		EXTENDED_203,
		EXTENDED_204,
		EXTENDED_205,
		EXTENDED_206,
		EXTENDED_207,
		EXTENDED_208,
		EXTENDED_209,
		EXTENDED_210,
		EXTENDED_211,
		EXTENDED_212,
		EXTENDED_213,
		EXTENDED_214,
		EXTENDED_215,
		EXTENDED_216,
		EXTENDED_217,
		EXTENDED_218,
		EXTENDED_219,
		EXTENDED_220,
		EXTENDED_221,
		EXTENDED_222,
		EXTENDED_223,
		EXTENDED_224,
		EXTENDED_225,
		EXTENDED_226,
		EXTENDED_227,
		EXTENDED_228,
		EXTENDED_229,
		EXTENDED_230,
		EXTENDED_231,
		EXTENDED_232,
		EXTENDED_233,
		EXTENDED_234,
		EXTENDED_235,
		EXTENDED_236,
		EXTENDED_237,
		EXTENDED_238,
		EXTENDED_239,
		EXTENDED_240,
		EXTENDED_241,
		EXTENDED_242,
		EXTENDED_243,
		EXTENDED_244,
		EXTENDED_245,
		EXTENDED_246,
		EXTENDED_247,
		EXTENDED_248,
		EXTENDED_249,
		EXTENDED_250,
		EXTENDED_251,
		EXTENDED_252,
		EXTENDED_253,
		EXTENDED_254,
		EXTENDED_255
	};
}

#endif