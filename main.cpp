// https://www.modernescpp.com/index.php/c-core-guidelines-type-erasure-with-templates
#include <vector>
#include <memory>
#include <iostream>

class at_short_message
{
public:
    explicit at_short_message(double rad)
        : radius{rad}
    {
    }

    void generate_delivery_receipt() const
    {
        std::cout<<"GenerateDeliveryReceipt\n";
    }

private:
    double radius;
};

class adr_short_message
{
public:
    explicit adr_short_message(double s)
        : side{s}
    {
    }

    bool is_timed_out() const
    {
        return false;
    }

private:
    double side;
};

class short_message
{
    private:
struct Concept
{
    virtual ~Concept() = default;

    virtual void do_serialize() const = 0;
    virtual void do_draw() const = 0;

    virtual bool check_is_timed_out() const  =0;
};

template<typename T>
struct  Model : public Concept
{
    Model(T short_message)
     : short_message_ {std::move(short_message)}
     {}

    void do_serialize() const override
    {
        serialize(short_message_);
    }

    void do_draw() const override
    {
        draw(short_message_);
    }

    bool check_is_timed_out() const  override
    {
        return is_timed_out(short_message_);
    }

     T short_message_;
};

friend void serialize(short_message const& shrt_msg)
{
    shrt_msg.pimpl->do_serialize();
}

friend void draw(short_message const& shrt_msg)
{
    shrt_msg.pimpl->do_draw();
}

friend bool is_timed_out(short_message const& shrt_msg)
{
    return shrt_msg.pimpl->check_is_timed_out();
}

std::unique_ptr<Concept> pimpl;

public:
    template<typename T>
    short_message(T short_message)
    : pimpl{std::make_unique<Model<T>> (std::move(short_message))}
    {}
};


void serialize(at_short_message const& c)
{
    std::cout<<"serialize at_short_message\n";
}
void draw(at_short_message const& c)
{
    std::cout<<"draw at_short_message\n";
}


void serialize(adr_short_message const& s)
{
    std::cout<<"serialize adr_short_message\n";
}

void draw(adr_short_message const& s)
{
    std::cout<<"draw adr_short_message\n";
}

bool is_timed_out(adr_short_message const& shrt_msg)
{
    return shrt_msg.is_timed_out();
}

// bool is_timed_out(at_short_message const& shrt_msg)
// {
//     // return shrt_msg.is_timed_out();
// }

void drawAllshort_messages(std::vector<short_message> const& short_messages)
{
    for(auto& short_message : short_messages)
        draw(short_message);
}

int main()
{
    using short_messages_t = std::vector<short_message>;

    short_messages_t short_messages;
    short_messages.emplace_back(at_short_message{2.0});
    short_messages.emplace_back(adr_short_message{1.5});
    short_messages.emplace_back(at_short_message{4.2});

    drawAllshort_messages(short_messages);

    auto adr = adr_short_message{1.5};
    adr.is_timed_out();

    return 0;
}