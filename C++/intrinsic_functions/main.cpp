#include <iostream>
#include <complex>

#include <immintrin.h>

#include <olcPixelGameEngine.h>

void CreateFractalNoComplex(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations) {

    double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
    double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

    double x_pos = frac_tl.x;
    double y_pos = frac_tl.y;

    int y_offset = 0;
    int row_size = ScreenWidth();

    int x, y, n;

    double cr = 0;
    double ci = 0;
    double zr = 0;
    double zi = 0;
    double re = 0;
    double im = 0;

    for (y = pix_tl.y; y < pix_br.y; y++)
    {
        x_pos = frac_tl.x;
        ci = y_pos;

        for (x = pix_tl.x; x < pix_br.x; x += 4)
        {
            cr = x_pos;
            zr = 0;
            zi = 0;

            n = 0;
            while ((zr * zr + zi * zi) < 4.0 && n < iterations)
            {
                re = zr * zr - zi * zi + cr;
                im = zr * zi * 2.0 + ci;
                zr = re;
                zi = im;
                n++;
            }

            pFractal[y_offset + x] = n;
            x_pos += x_scale;
        }

        y_pos += y_scale;
        y_offset += row_size;
    }

}

void CreateFractalIntrinsics(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations) {

    double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
    double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

    int x, y;
    double y_pos = frac_tl.y;
    int y_offset = 0;
    int row_size = ScreenWidth();

    // 64-bit "integer" registers
    __m256i _n, _iterations, _mask2, _c, _one;

    _iterations = _mm256_set1_epi64x(iterations);
    _one = _mm256_set1_epi64x(1);

    // 64-bit "double" registers
    __m256d _zr, _zi, _cr, _ci, _a, _b, _zr2, _zi2, _two, _four, _mask1;

    // two = |2.0|2.0|2.0|2.0|
    _two = _mm256_set1_pd(2.0);

    // four = |4.0|4.0|4.0|4.0|
    _four = _mm256_set1_pd(4.0);

    // Additional registers for x-axis calculations
    __m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump;
    _x_scale = _mm256_set1_pd(x_scale);
    _x_jump = _mm256_set1_pd(x_scale * 4);
    _x_pos_offsets = _mm256_set_pd(0, 1, 2, 3); // This lets us set each
    _x_pos_offsets = _mm256_mul_pd(_x_pos_offsets, _x_scale);

    for (y = pix_tl.y; y < pix_br.y; y++)
    {
        // Rest x position
        _a = _mm256_set1_pd(frac_tl.x);
        _x_pos = _mm256_add_pd(_a, _x_pos_offsets);
        
        _ci = _mm256_set1_pd(y_pos);

        for (x = pix_tl.x; x < pix_br.x; x += 4) // Note: incrementing by 4 because AVX2-256 gives us 4 64 bit doubles
        {
            _cr = _x_pos;

            // Initialize a few things to 0
            _zr = _mm256_setzero_pd();
            _zi = _mm256_setzero_pd();

            _n = _mm256_setzero_si256();

            // So we'll be doing groups of 4 at a time because of these intrinsics

            repeat:

            // First attack the while loop
            /*
            int n = 0;
            while (abs(z) < 2.0 && n < iterations) {
                z = (z*z)+c;
                n++
            }
            */

            // Normal: z = (z*z)+c;
            // Manual: a = zr*zr - zi*zi + cr;
            //         b = zr*zi*2.0 + ci;
            //         zr = a;
            //         zi = b;

            // zr^2 = zr*zr
            _zr2 = _mm256_mul_pd(_zr, _zr); // zr * zr

            // zi^2 = zi*zi
            _zi2 = _mm256_mul_pd(_zi, _zi); // zi * zi

            // a = zr^2 - zi^2
            _a = _mm256_sub_pd(_zr2, _zi2); // a = (zr * zr) - (zi * zi)

            // a = a + cr
            _a = _mm256_add_pd(_a, _cr); // a = (zr*zr) - (zi*zi) + cr

            // b = zr*zi
            _b = _mm256_mul_pd(_zr, _zi); // b = zr * zi

            // b = b * 2.0 + ci
            // b = b * |2.0|2.0|2.0|2.0| + ci
            _b = _mm256_fmadd_pd(_b, _two, _ci); // b = (zr*zi)*2.0 + ci

            // zr = a
            _zr = _a; // zr = a

            // zi = b
            _zi = _b; // zi = b


            // Now attack the while loop. These are the tricky conditional things
            // where intrinsic masks give us the speed boost
            // Normal: while (abs(z) < 2.0 && n < iterations)
            // Manual: while ((zr*zr+zi*zi) < 4.0 && n < iterations)

            // a = zr^2 + zi^2
            _a = _mm256_add_pd(_zr2, _zi2); // a = (zr * zr) + (zi * zi)

            // m1 = if (a < 4.0)
            // m1 = |if (a[3] < 4.0)|if (a[2] < 4.0)|if (a[1] < 4.0)|if (a[0] < 4.0)|
            // m1 = |00...00|11...11|00...00|11...11|
            _mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ); // Compare Less Than

            // m2 = if (iterations > n)
            // m2 = |00...00|11...11|00...00|11...11|
            _mask2 = _mm256_cmpgt_epi64(_iterations, _n);

            // m2 = m2 AND m1 = if (a < 4.0 && iterations > n)
            _mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));

            // c = |(int)1|(int)1|(int)1|(int)1|
            //
            //  c =    |00..01|00...01|00...01|00...01|
            // m2 = AND|00..00|00...00|11...11|00...00|
            //  c =    |00..00|00...00|00...00|00...00|
            //
            // c = |(int)0|(int)0|(int)1|(int)0|
            
            // n = n + c
            // 
            // n =    |00..24|00...13|00...08|00...21|
            // c = AND|00..00|00...00|11...11|00...00|
            // n =    |00..24|00...13|00...09|00...21| (Increment only applied to 'enabled' element)
            //
            _n = _mm256_add_epi64(_n, _c);

            // Since we're using an assembly-like thing, we may as well use goto
            // if ((zr*zr + zi*zi) < 4.0 && n < iterations) goto repeat
            // i.e. if our mask has any elements that are 1
            // |00...00|00...00|11...11|00...00|
            // |   0   |   0   |   1   |   0   | = 0b0010 = 2
            // so ... if (2 > 0) goto repeat
            if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0)
                goto repeat;

            // Unpack complicated vector type in to integers
#if defined(__WIN32)
            pFractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
            pFractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
            pFractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
            pFractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
#endif
#if defined(__linux__)
            pFractal[y_offset + x + 0] = int(_n[3]);
            pFractal[y_offset + x + 1] = int(_n[2]);
            pFractal[y_offset + x + 2] = int(_n[1]);
            pFractal[y_offset + x + 3] = int(_n[0]);
#endif

            // If you ever want to know what intrinsic functions are 
            // available, then you can go to Google. Look for
            // "x86 Intrinsics List"
        }
    }

}