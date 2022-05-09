#pragma once
// used: std::array
#include <array>

enum
{
	COLOR_R = 0,
	COLOR_G = 1,
	COLOR_B = 2,
	COLOR_A = 3
};

struct ColorRGBExp32
{
	std::uint8_t r, g, b;
	char exponent;
};

class Color
{
public:
	Color() = default;

	/* default color constructor (in: 0 - 255) */
	constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) :
		arrColor({ r, g, b, a }) { }

	constexpr Color(int r, int g, int b, int a = 255) :
		arrColor({ static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b), static_cast<std::uint8_t>(a) }) { }

	/* float color constructor (in: 0.0 - 1.0) */
	constexpr Color(float r, float g, float b, float a = 1.0f) :
		arrColor({ static_cast<std::uint8_t>(r * 255.f), static_cast<std::uint8_t>(g * 255.f), static_cast<std::uint8_t>(b * 255.f), static_cast<std::uint8_t>(a * 255.f) }) { }

	/* output color to given variables */
	void Get(std::uint8_t& r, std::uint8_t& g, std::uint8_t& b, std::uint8_t& a) const
	{
		r = arrColor[COLOR_R];
		g = arrColor[COLOR_G];
		b = arrColor[COLOR_B];
		a = arrColor[COLOR_A];
	}


	std::uint8_t& operator[](const std::size_t i)
	{
		return this->arrColor[i];
	}

	const std::uint8_t& operator[](const std::size_t i) const
	{
		return this->arrColor[i];
	}

	bool operator==(const Color& colSecond) const
	{
		return this->arrColor == colSecond.arrColor;
	}

	bool operator!=(const Color& colSecond) const
	{
		return !(operator==(colSecond));
	}

	Color& operator=(const Color& colFrom)
	{
		arrColor[COLOR_R] = colFrom.arrColor[COLOR_R];
		arrColor[COLOR_G] = colFrom.arrColor[COLOR_G];
		arrColor[COLOR_B] = colFrom.arrColor[COLOR_B];
		arrColor[COLOR_A] = colFrom.arrColor[COLOR_A];
		return *this;
	}

	/* returns certain R/G/B/A value */
	template <std::size_t N>
	[[nodiscard]] std::uint8_t Get() const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "given index is out of range");
		return arrColor[N];
	}

	/* returns copy of color with changed certain R/G/B/A value to given value */
	template <std::size_t N>
	[[nodiscard]] Color Set(const std::uint8_t nValue) const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "given index is out of range");

		Color colCopy = *this;
		colCopy.arrColor[N] = nValue;
		return colCopy;
	}

	/* returns copy of color with multiplied certain R/G/B/A value by given value */
	template <std::size_t N>
	[[nodiscard]] Color Multiplier(const float flValue) const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "given index is out of range");

		Color colCopy = *this;
		colCopy.arrColor[N] = static_cast<std::uint8_t>(static_cast<float>(colCopy.arrColor[N]) * flValue);
		return colCopy;
	}

	/* returns copy of color with divided certain R/G/B/A value by given value */
	template <std::size_t N>
	[[nodiscard]] Color Divider(const int iValue) const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "given index is out of range");

		Color colCopy = *this;
		colCopy.arrColor[N] /= iValue;
		return colCopy;
	}

	/* returns certain R/G/B/A float value (in: 0 - 255, out: 0.0 - 1.0) */
	template <std::size_t N>
	[[nodiscard]] float Base() const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "given index is out of range");
		return arrColor[N] / 255.f;
	}

	/* convert color to float array (in: 0 - 255, out: 0.0 - 1.0) */
	[[nodiscard]] std::array<float, 3U> Base() const
	{
		std::array<float, 3U> arrBaseColor = { };
		arrBaseColor[COLOR_R] = this->Base<COLOR_R>();
		arrBaseColor[COLOR_G] = this->Base<COLOR_G>();
		arrBaseColor[COLOR_B] = this->Base<COLOR_B>();
		return arrBaseColor;
	}

	/* set color from float array (in: 0.0 - 1.0, out: 0 - 255) */
	static Color FromBase3(float arrBase[3])
	{
		return Color(arrBase[0], arrBase[1], arrBase[2]);
	}

	/* convert color to float array w/ alpha (in: 0 - 255, out: 0.0 - 1.0) */
	[[nodiscard]] std::array<float, 4U> BaseAlpha() const
	{
		std::array<float, 4U> arrBaseColor = { };
		arrBaseColor[COLOR_R] = this->Base<COLOR_R>();
		arrBaseColor[COLOR_G] = this->Base<COLOR_G>();
		arrBaseColor[COLOR_B] = this->Base<COLOR_B>();
		arrBaseColor[COLOR_A] = this->Base<COLOR_A>();
		return arrBaseColor;
	}

	/* set color from float array w/ alpha (in: 0.0 - 1.0, out: 0 - 255) */
	static Color FromBase4(float arrBase[4])
	{
		return Color(arrBase[0], arrBase[1], arrBase[2], arrBase[3]);
	}


private:
	std::array<std::uint8_t, 4U> arrColor;
};
