#ifndef SMARTHATA_PAGEABLE_H
#define SMARTHATA_PAGEABLE_H

#include "web.h"

class Pageable {
public:

    Pageable(const String &name) : name(name) {}

    String getName() const {
        return name;
    };

    virtual void process() {};

    virtual void write(Print &p) const = 0;

    virtual const uint16_t getLength() const = 0;

private:
    String name;
};

class FunctionPage : public Pageable {
public:
    FunctionPage(const String &name, String (*func)()) : Pageable(name), func(func) {}

    void process() override {
        content = func();
    }

    void write(Print &p) const override {
        p.print(content);
    }

    const uint16_t getLength() const override {
        return content.length();
    }

private:
    String content;

    String (*func)();
};

class Page : public Pageable {
public:
    Page(PGM_P name, PGM_P content, const uint16_t length) : Pageable(String((__FlashStringHelper *) name)),
                                                             content(content),
                                                             length(length) {};

    void write(Print &p) const override {
        for (unsigned int i = 0; i < length; ++i) {
            p.print((char) pgm_read_byte_near(content + i));
        }
    }

    const uint16_t getLength() const override {
        return length;
    }

private:
    PGM_P content;
    const uint16_t length;
};

#endif
