namespace Ariadne
{
    class GeoPoint
    {
    public:
        double Lat;
        double Long;
        double getOffset(GeoPoint start, double xmOffset, double ymOffset);
    };
}