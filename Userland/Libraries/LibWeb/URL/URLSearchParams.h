/*
 * Copyright (c) 2021, Idan Horowitz <idan.horowitz@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/URL.h>
#include <AK/Vector.h>
#include <LibWeb/WebIDL/ExceptionOr.h>

namespace Web::URL {

struct QueryParam {
    String name;
    String value;
};
ErrorOr<String> url_encode(Vector<QueryParam> const&, AK::URL::PercentEncodeSet);
ErrorOr<Vector<QueryParam>> url_decode(StringView);

class URLSearchParams : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(URLSearchParams, Bindings::PlatformObject);

public:
    static WebIDL::ExceptionOr<JS::NonnullGCPtr<URLSearchParams>> create(JS::Realm&, Vector<QueryParam> list);
    static WebIDL::ExceptionOr<JS::NonnullGCPtr<URLSearchParams>> construct_impl(JS::Realm&, Variant<Vector<Vector<String>>, OrderedHashMap<String, String>, String> const& init);

    virtual ~URLSearchParams() override;

    size_t size() const;
    WebIDL::ExceptionOr<void> append(String const& name, String const& value);
    WebIDL::ExceptionOr<void> delete_(String const& name);
    Optional<String> get(String const& name);
    WebIDL::ExceptionOr<Vector<String>> get_all(String const& name);
    bool has(String const& name);
    WebIDL::ExceptionOr<void> set(String const& name, String const& value);

    WebIDL::ExceptionOr<void> sort();

    WebIDL::ExceptionOr<String> to_string() const;

    using ForEachCallback = Function<JS::ThrowCompletionOr<void>(String const&, String const&)>;
    JS::ThrowCompletionOr<void> for_each(ForEachCallback);

private:
    friend class URL;
    friend class URLSearchParamsIterator;

    URLSearchParams(JS::Realm&, Vector<QueryParam> list);

    virtual JS::ThrowCompletionOr<void> initialize(JS::Realm&) override;
    virtual void visit_edges(Cell::Visitor&) override;

    WebIDL::ExceptionOr<void> update();

    Vector<QueryParam> m_list;
    JS::GCPtr<URL> m_url;
};

}
