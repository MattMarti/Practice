#include <chrono>
#include <cmath>
#include <complex>
#include <thread>
#include <condition_variable>
#include <mutex>

#include <immintrin.h>

#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>

const int nMaxThreads = 32;

class olcFractalExplorer : public olc::PixelGameEngine
{
public:
    olcFractalExplorer()
    {
        sAppName = "Brute Force Processing";
    }

    int* pFractal = nullptr;
    int nMode = 0;
    int nIterations = 64;

    // Pan & Zoom Variables
    olc::vd2d vOffset = { 0.0, 0.0 };
    olc::vd2d vStartPan = { 0.0, 0.0 };
    olc::vd2d vScale = { 1280.0 / 2.0, 720.0 };

    // Convert coordinates from World Space to Screen Space
    void WorldToScreen(const olc::vd2d& v, olc::vi2d& n)
    {
        n.x = (int)((v.x - vOffset.x) * vScale.x);
        n.y = (int)((v.y - vOffset.y) * vScale.y);
    }

    // Convert coordinates from Screen Space to World Space
    void ScreenToWorld(const olc::vi2d& n, olc::vd2d& v)
    {
        v.x = (double)(n.x) / vScale.x + vOffset.x;
        v.y = (double)(n.y) / vScale.y + vOffset.y;
    }

    bool OnUserCreate() override
    {
        pFractal = new int[ScreenWidth() * ScreenHeight()]{ 0 };
        InitializeThreadPool();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Get mouse location this frame
        olc::vd2d vMouse = { (double)GetMouseX(), (double)GetMouseY() };

        // Handle Pan & Zoom
        if (GetMouse(2).bPressed)
        {
            vStartPan = vMouse;
        }

        if (GetMouse(2).bHeld)
        {
            vOffset -= (vMouse - vStartPan) / vScale;
            vStartPan = vMouse;
        }

        olc::vd2d vMouseBeforeZoom;
        ScreenToWorld(vMouse, vMouseBeforeZoom);

        if (GetKey(olc::Key::Q).bHeld || GetMouseWheel() > 0) vScale *= 1.1;
        if (GetKey(olc::Key::A).bHeld || GetMouseWheel() < 0) vScale *= 0.9;

        olc::vd2d vMouseAfterZoom;
        ScreenToWorld(vMouse, vMouseAfterZoom);
        vOffset += (vMouseBeforeZoom - vMouseAfterZoom);

        olc::vi2d pix_tl = { 0,0 };
        olc::vi2d pix_br = { ScreenWidth(), ScreenHeight() };
        olc::vd2d frac_tl = { -2.0, -1.0 };
        olc::vd2d frac_br = { 1.0, 1.0 };

        ScreenToWorld(pix_tl, frac_tl);
        ScreenToWorld(pix_br, frac_br);

        // Handle User Input
        if (GetKey(olc::K1).bPressed) nMode = 0;
        if (GetKey(olc::K2).bPressed) nMode = 1;
        if (GetKey(olc::K3).bPressed) nMode = 2;
        if (GetKey(olc::K4).bPressed) nMode = 3;
        if (GetKey(olc::K5).bPressed) nMode = 4;
        if (GetKey(olc::K6).bPressed) nMode = 5;
        if (GetKey(olc::K7).bPressed) nMode = 6;
        if (GetKey(olc::UP).bPressed) nIterations += 64;
        if (GetKey(olc::DOWN).bPressed) nIterations -= 64;
        if (nIterations < 64) nIterations = 64;

        // START TIMING
        auto tp1 = std::chrono::high_resolution_clock::now();

        // Do the computation
        switch (nMode)
        {
        case 0: CreateFractalBasic(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 1: CreateFractalPreCalculate(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 2: CreateFractalNoComplex(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 3: CreateFractalNoAbs(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 4: CreateFractalIntrinsics(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 5: CreateFractalThreading(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 6: CreateFractalThreadPool(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        }

        // STOP TIMING
        auto tp2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = tp2 - tp1;

        // Render to screen
        for (int y = 0; y < ScreenHeight(); y++)
        {
            for (int x = 0; x < ScreenWidth(); x++)
            {
                int i = pFractal[y * ScreenWidth() + x];
                float n = (float)i;
                float a = 0.1f;

                // Eriksonn - Wonderful Magic Fractal Oddball Man
                Draw(x, y, olc::PixelF(
                    0.5f * sin(a * n) + 0.5f,
                    0.5f * sin(a * n + 2.094f) + 0.5f,
                    0.5f * sin(a * n + 4.188f) + 0.5f
                ));
            }
        }

        // Render UI
        switch (nMode)
        {
        case 0: DrawString(0, 0, "1: Naive Method", olc::WHITE, 3); break;
        case 1: DrawString(0, 0, "2: Precalc Method", olc::WHITE, 3); break;
        case 2: DrawString(0, 0, "3: No STD Complex Method", olc::WHITE, 3); break;
        case 3: DrawString(0, 0, "4: No abs(z) Method", olc::WHITE, 3); break;
        case 4: DrawString(0, 0, "5: Vector Extensions Method", olc::WHITE, 3); break;
        case 5: DrawString(0, 0, "6: Threading Method", olc::WHITE, 3); break;
        case 6: DrawString(0, 0, "7: Thread Pool Method", olc::WHITE, 3); break;
        }

        DrawString(0, 30, "Time Taken: " + std::to_string(elapsedTime.count()) + "s", olc::WHITE, 3);
        DrawString(0, 60, "Iterations: " + std::to_string(nIterations), olc::WHITE, 3);
        return !(GetKey(olc::Key::ESCAPE).bPressed);
    }

    // Let's implement a basic fractal function
    void CreateFractalBasic(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
    {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        for (int y = pix_tl.y; y < pix_br.y; y++)
        {
            for (int x = pix_tl.x; x < pix_br.x; x++)
            {
                std::complex<double> c(x * x_scale + frac_tl.x, y * y_scale + frac_tl.y);
                std::complex<double> z(0, 0);

                int n = 0;
                while (abs(z) < 2.0 && n < iterations)
                {
                    z = (z * z) + c;
                    n++;
                }

                pFractal[y * ScreenWidth() + x] = n;
            }
        }
    }

    // Let's try to be smarter than the compiler
    void CreateFractalPreCalculate(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
    {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        int x, y, n;
        std::complex<double> c, z;

        double x_pos = frac_tl.x;
        double y_pos = frac_tl.y;
        int y_offset = 0;
        int row_size = ScreenWidth();

        for (int y = pix_tl.y; y < pix_br.y; y++)
        {
            x_pos = frac_tl.x;
            for (int x = pix_tl.x; x < pix_br.x; x++)
            {
                c = { x_pos, y_pos };
                z = { 0, 0 };

                n = 0;
                while (abs(z) < 2.0 && n < iterations)
                {
                    z = (z * z) + c;
                    n++;
                }

                x_pos += x_scale;

                pFractal[y_offset + x] = n;
            }
            y_pos += y_scale;
            y_offset += row_size;
        }
    }

    // Let's try to not use std::complex
    void CreateFractalNoComplex(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
    {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        int x, y, n;
        double cr = 0;
        double ci = 0;
        double zr = 0;
        double zi = 0;
        double re = 0;
        double im = 0;

        double x_pos = frac_tl.x;
        double y_pos = frac_tl.y;

        int y_offset = 0;
        int row_size = ScreenWidth();

        for (y = pix_tl.y; y < pix_br.y; y++)
        {
            ci = y_pos;
            x_pos = frac_tl.x;
            for (x = pix_tl.x; x < pix_br.x; x++)
            {
                cr = x_pos;
                zr = 0;
                zi = 0;

                n = 0;
                while ((zr * zr + zi * zi) < 4.0 && n < iterations) // Not using the square root in abs(complex) is probably responsible for increased speed
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

    // Go back to using std::complex, but don't use the abs() because it has square root
    void CreateFractalNoAbs(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
    {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        int row_size = ScreenWidth();

        for (int y = pix_tl.y; y < pix_br.y; y++)
        {
            for (int x = pix_tl.x; x < pix_br.x; x++)
            {
                std::complex<double> c(x * x_scale + frac_tl.x, y * y_scale + frac_tl.y);
                std::complex<double> z(0, 0);

                int n = 0;
                while (norm(z) < 4.0 && n < iterations)
                {
                    z = (z * z) + c;
                    n++;
                }

                pFractal[y * row_size + x] = n;
            }
        }
    }

    // AVX-256 chip parallelism. We'll do 4 rows at a time. This should speed us up 4x
    void CreateFractalIntrinsics(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
    {
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
                //
                // m2 =    |00..00|11...11|11...11|00...00|
                // m1 = AND|11..11|00...00|11...11|00...00|
                // m2 =    |00..00|00...00|11...11|00...00|
                _mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));

                // c = |(int)1|(int)1|(int)1|(int)1| AND m2
                //
                //  c =    |00..01|00...01|00...01|00...01|
                // m2 = AND|00..00|00...00|11...11|00...00|
                //  c =    |00..00|00...00|00...01|00...00|
                //
                // c = |(int)0|(int)0|(int)1|(int)0|
                _c = _mm256_and_si256(_one, _mask2);

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
#if defined(_WIN32)
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
                _x_pos = _mm256_add_pd(_x_pos, _x_jump);

                // If you ever want to know what intrinsic functions are 
                // available, then you can go to Google. Look for
                // "x86 Intrinsics List"
            }
            y_pos += y_scale;
            y_offset += row_size;
        }
    }

    // Let's try threading to speed this up! Use all our CPUs
    void CreateFractalThreading(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
    {
        int nSectionWidth = (pix_br.x - pix_tl.x) / nMaxThreads;
        double dFractalWidth = (frac_br.x - frac_tl.x) / double(nMaxThreads);

        std::thread t[nMaxThreads];
        for (size_t i = 0; i < nMaxThreads; i++)
        {
            t[i] = std::thread(
                &olcFractalExplorer::CreateFractalIntrinsics,
                this,
                olc::vi2d(pix_tl.x + nSectionWidth * (i), pix_tl.y),
                olc::vi2d(pix_tl.x + nSectionWidth * (i + 1), pix_br.y),
                olc::vd2d(frac_tl.x + dFractalWidth * double(i), frac_tl.y),
                olc::vd2d(frac_tl.x + dFractalWidth * double(i + 1), frac_br.y),
                iterations);
        }

        for (size_t i = 0; i < nMaxThreads; i++)
        {
            t[i].join();
        }
    }

    // Use a thread pool to eliminate the overhead of entering/exiting threads
    struct WorkerThread
    {
        olc::vi2d pix_tl = { 0,0 };
        olc::vi2d pix_br = { 0,0 };
        olc::vd2d frac_tl = { 0, 0 };
        olc::vd2d frac_br = { 0, 0 };
        int iterations = 0;
        std::condition_variable cvStart;
        bool alive = true;
        std::mutex mux;
        int screen_width = 0;
        int* fractal = nullptr;
        
        olcFractalExplorer* owner = nullptr;

        std::thread thread;

        void Start(const olc::vi2d& ptl, const olc::vi2d& pbr, const olc::vd2d& ftl, const olc::vd2d& fbr, int it)
        {
            pix_tl = ptl;
            pix_br = pbr;
            frac_tl = ftl;
            frac_br = fbr;
            iterations = it;
            std::unique_lock<std::mutex> lm(mux);
            cvStart.notify_one();
        }

        void CreateFractal()
        {
            while (alive)
            {
                std::unique_lock<std::mutex> lm(mux);
                cvStart.wait(lm);

                owner->CreateFractalIntrinsics(pix_tl, pix_br, frac_tl, frac_br, iterations);

                owner->nWorkerComplete++;
            }
        }
    };
    WorkerThread workers[nMaxThreads];
    std::atomic<int> nWorkerComplete; // Signal that a worker thread has completed it's part of the fractal

    void InitializeThreadPool()
    {
        for (int i = 0; i < nMaxThreads; i++)
        {
            workers[i].owner = this;
            workers[i].alive = true;
            workers[i].fractal = pFractal;
            workers[i].screen_width = ScreenWidth();
            workers[i].thread = std::thread(&WorkerThread::CreateFractal, &workers[i]);
        }
    }
    void CreateFractalThreadPool(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
    {
        int nSectionWidth = (pix_br.x - pix_tl.x) / nMaxThreads;
        double dFractalWidth = (frac_br.x - frac_tl.x) / double(nMaxThreads);

        nWorkerComplete = 0;

        for (size_t i = 0; i < nMaxThreads; i++)
        {
            workers[i].Start(
                olc::vi2d(pix_tl.x + nSectionWidth * (i), pix_tl.y),
                olc::vi2d(pix_tl.x + nSectionWidth * (i + 1), pix_br.y),
                olc::vd2d(frac_tl.x + dFractalWidth * double(i), frac_tl.y),
                olc::vd2d(frac_tl.x + dFractalWidth * double(i + 1), frac_br.y),
                iterations);
        }

        while (nWorkerComplete < nMaxThreads) // Wait for all workers to complete
        { } // Probabl a better way to do this
    }
};

int main()
{
    olcFractalExplorer demo;
    if (demo.Construct(1280, 720, 1, 1, false, false))
        demo.Start();
    return 0;
}