#include <stdint.h>
#include <stdio.h>

#include <zephyr/kernel.h>

#include "quickjs.h"

static int report_exception(JSContext *ctx, const char *stage)
{
    JSValue exception = JS_GetException(ctx);
    const char *message = JS_ToCString(ctx, exception);

    printk("QuickJS %s failed: %s\n", stage, message ? message : "unknown exception");
    JS_FreeCString(ctx, message);
    JS_FreeValue(ctx, exception);
    return -1;
}

int main(void)
{
    JSRuntime *runtime = JS_NewRuntime();
    JSContext *context;
    JSContext *job_context;
    JSValue value;
    int32_t result;

    if (runtime == NULL) {
        printk("QuickJS runtime allocation failed\n");
        return 1;
    }
    JS_SetCanBlock(runtime, true);

    context = JS_NewContext(runtime);
    if (context == NULL) {
        printk("QuickJS context allocation failed\n");
        JS_FreeRuntime(runtime);
        return 1;
    }

    value = JS_Eval(context,
                    "globalThis.answer = 0;"
                    "Promise.resolve(40).then(v => { globalThis.answer = v + 2; });",
                    sizeof("globalThis.answer = 0;"
                           "Promise.resolve(40).then(v => { globalThis.answer = v + 2; });") - 1,
                    "<zephyr-promise>", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(value)) {
        JS_FreeValue(context, value);
        report_exception(context, "promise evaluation");
        goto fail;
    }
    JS_FreeValue(context, value);

    while (JS_IsJobPending(runtime)) {
        if (JS_ExecutePendingJob(runtime, &job_context) < 0) {
            report_exception(job_context, "pending job");
            goto fail;
        }
    }

    static const char atomics_script[] =
        "(() => {"
        " const sab = new SharedArrayBuffer(32);"
        " const u8 = new Uint8Array(sab);"
        " const i16 = new Int16Array(sab);"
        " const i32 = new Int32Array(sab);"
        " const i64 = new BigInt64Array(sab);"
        " if (Atomics.add(i32, 0, 40) !== 0) return -1;"
        " if (Atomics.compareExchange(i32, 0, 40, 42) !== 40) return -2;"
        " if (Atomics.wait(i32, 1, 0, 1) !== 'timed-out') return -3;"
        " if (Atomics.exchange(u8, 16, 5) !== 0) return -4;"
        " if (Atomics.or(u8, 16, 2) !== 5) return -5;"
        " if (Atomics.store(i16, 9, 9) !== 9) return -6;"
        " if (Atomics.xor(i16, 9, 5) !== 9) return -7;"
        " if (Atomics.store(i64, 1, 7n) !== 7n) return -8;"
        " if (Atomics.add(i64, 1, 2n) !== 7n) return -9;"
        " if (Atomics.load(i64, 1) !== 9n) return -10;"
        " return globalThis.answer;"
        "})()";

    value = JS_Eval(context, atomics_script, sizeof(atomics_script) - 1,
                    "<zephyr-atomics>", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(value)) {
        JS_FreeValue(context, value);
        report_exception(context, "Atomics evaluation");
        goto fail;
    }
    if (JS_ToInt32(context, &result, value) < 0) {
        JS_FreeValue(context, value);
        report_exception(context, "result conversion");
        goto fail;
    }
    JS_FreeValue(context, value);

    printk("QuickJS Zephyr returned %ld\n", (long)result);
    if (result != 42) {
        goto fail;
    }

    printk("QuickJS Zephyr: PASS\n");
    JS_FreeContext(context);
    JS_FreeRuntime(runtime);
    return 0;

fail:
    JS_FreeContext(context);
    JS_FreeRuntime(runtime);
    return 1;
}
