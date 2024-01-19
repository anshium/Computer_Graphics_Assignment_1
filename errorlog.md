# Errors
- I converted the slab test code incorrectly. Had a max instead of a min in tz2 calculation.
```float tz2 = (this->min.z - ray.o.z) / ray.d.z;```
instead of 
```float tz2 = (this->max.z - ray.o.z) / ray.d.z;```

This caused a partial image to be form. Not sure why the glass was half made. Try that with option 1 and the errorneous code and we will observe the same.

- Changed the rayIntersect function to that similar on the blog. That seems to partially work. There are some objects that did not render though. Looking at them.