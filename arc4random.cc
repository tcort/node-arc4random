/*
 * Copyright (c) 2014, 2015 Thomas Cort <linuxgeek@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define __STDC_LIMIT_MACROS

#include <node.h>
#include <nan.h>

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef USE_LIBBSD
#include <bsd/stdlib.h>
#endif

NAN_METHOD(node_arc4random) {
    Nan::HandleScope scope;

    if (info.Length() != 0) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    info.GetReturnValue().Set(Nan::New<v8::Number>(arc4random()));
}

NAN_METHOD(node_arc4random_buf) {
    Nan::HandleScope scope;

    if (info.Length() != 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!node::Buffer::HasInstance(info[0]) || !info[1]->IsNumber() || isnan(info[1]->NumberValue()) || info[1]->IntegerValue() < 0 || info[1]->IntegerValue() > UINT32_MAX) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    v8::Handle<v8::Value> bufferObj = info[0]->ToObject();
    char*  bufferData = node::Buffer::Data(bufferObj);
    size_t bufferLength = node::Buffer::Length(bufferObj);
    size_t nbytes = info[1]->IntegerValue();

    if (bufferLength < nbytes) {
        Nan::ThrowRangeError("Trying to write outside buffer length");
        return;
    }

    arc4random_buf(bufferData, nbytes);
    return;
}

NAN_METHOD(node_arc4random_uniform) {
    Nan::HandleScope scope;

    if (info.Length() != 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsNumber() || isnan(info[0]->NumberValue()) || info[0]->IntegerValue() < 0 || info[0]->IntegerValue() > UINT32_MAX) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    info.GetReturnValue().Set(Nan::New<v8::Number>(arc4random_uniform(info[0]->Uint32Value())));
}

NAN_MODULE_INIT(init) {
    Nan::Export(target, "arc4random", node_arc4random);
    Nan::Export(target, "arc4random_buf", node_arc4random_buf);
    Nan::Export(target, "arc4random_uniform", node_arc4random_uniform);
}

NODE_MODULE(arc4random, init)
