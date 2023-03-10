package com.cloudnotes.service.service;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.Catalogue;
import org.springframework.web.bind.annotation.RequestBody;

import java.util.List;

public interface CatalogService {
    public List<Catalogue> find(Integer userId);
    public Result update(Catalogue catalogue);
    public Result create(@RequestBody Catalogue catalogue);
    public Result delete(Integer id);
}
